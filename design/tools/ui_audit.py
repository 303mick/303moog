#!/usr/bin/env python3
"""ui_audit.py - objective polish checks for an HTML UI mock (part of UI Agents).

Usage:
  python3 design/tools/ui_audit.py PATH_OR_URL [--out DIR] [--viewports 1366x768@1,1920x1080@1,1440x900@2]
                                   [--min-font 10] [--min-target 24] [--base 4] [--allow-scroll]
  python3 design/tools/ui_audit.py contrast '#e7eefc' '#0a1020'

Writes screenshots (plus grayscale and blurred "squint" versions when Pillow is installed),
audit.json and audit.md into --out. Exit code 1 if any ERROR is found.
"""
import argparse, json, pathlib, sys

# ---------- contrast maths ----------
def _lin(c):
    c /= 255.0
    return c / 12.92 if c <= 0.03928 else ((c + 0.055) / 1.055) ** 2.4

def luminance(rgb):
    r, g, b = rgb
    return 0.2126 * _lin(r) + 0.7152 * _lin(g) + 0.0722 * _lin(b)

def ratio(a, b):
    la, lb = luminance(a), luminance(b)
    hi, lo = max(la, lb), min(la, lb)
    return (hi + 0.05) / (lo + 0.05)

def parse_hex(h):
    h = h.strip().lstrip('#')
    if len(h) == 3:
        h = ''.join(c * 2 for c in h)
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))

def contrast_cli(fg, bg):
    r = ratio(parse_hex(fg), parse_hex(bg))
    print(f"{fg} on {bg}: {r:.2f}:1  body(4.5) {'PASS' if r >= 4.5 else 'FAIL'}  large/UI(3.0) {'PASS' if r >= 3 else 'FAIL'}  AAA body(7) {'PASS' if r >= 7 else 'FAIL'}")
    return 0 if r >= 4.5 else 1

# ---------- in-page probe ----------
PROBE = r"""
(opts) => {
  const out = {texts: [], small: [], targets: [], fontSizes: {}, families: {}, weights: {}, radii: {}, textColors: {},
               spacing: {}, offScale: {}, css: {}, overflow: {}};
  const parse = c => { const m = c && c.match(/rgba?\(([^)]+)\)/); if (!m) return null;
    const p = m[1].split(/[ ,\/]+/).filter(Boolean).map(Number);
    return {r: p[0], g: p[1], b: p[2], a: p.length > 3 ? p[3] : 1}; };
  const lin = c => { c /= 255; return c <= 0.03928 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4); };
  const lum = c => 0.2126 * lin(c.r) + 0.7152 * lin(c.g) + 0.0722 * lin(c.b);
  const ratio = (a, b) => { const x = lum(a), y = lum(b); return (Math.max(x, y) + 0.05) / (Math.min(x, y) + 0.05); };
  const over = (top, bot) => { const a = top.a + bot.a * (1 - top.a);
    if (a === 0) return {r: 255, g: 255, b: 255, a: 1};
    const f = k => (top[k] * top.a + bot[k] * bot.a * (1 - top.a)) / a;
    return {r: f('r'), g: f('g'), b: f('b'), a: a}; };
  const hex = c => '#' + [c.r, c.g, c.b].map(v => Math.round(v).toString(16).padStart(2, '0')).join('');
  const bump = (o, k) => { o[k] = (o[k] || 0) + 1; };
  const sel = el => el.tagName.toLowerCase() + (el.id ? '#' + el.id : '') +
      (el.classList && el.classList.length ? '.' + [...el.classList].slice(0, 2).join('.') : '');
  const visible = (el, cs, r) => cs.display !== 'none' && cs.visibility !== 'hidden' && parseFloat(cs.opacity) > 0.01 && r.width > 0 && r.height > 0;
  const scaleOK = v => { if (Math.abs(v) < 0.5) return true; const a = Math.abs(v);
    if ([1, 2, 6].some(x => Math.abs(a - x) < 0.5)) return true;
    return Math.abs(a / opts.base - Math.round(a / opts.base)) * opts.base < 0.5; };

  const all = [...document.querySelectorAll('body *')].slice(0, 6000);
  for (const el of all) {
    const cs = getComputedStyle(el), r = el.getBoundingClientRect();
    if (!visible(el, cs, r)) continue;
    const isSvg = el instanceof SVGElement;

    // radii
    if (!isSvg) { const br = cs.borderTopLeftRadius; if (br && br !== '0px') bump(out.radii, br); }

    // spacing
    if (!isSvg) {
      const props = ['paddingTop', 'paddingRight', 'paddingBottom', 'paddingLeft', 'marginTop', 'marginRight', 'marginBottom', 'marginLeft'];
      if (cs.display.includes('flex') || cs.display.includes('grid')) props.push('rowGap', 'columnGap');
      for (const p of props) { const v = parseFloat(cs[p]); if (isNaN(v) || cs[p] === 'normal') continue;
        bump(out.spacing, 'n'); if (!scaleOK(v)) bump(out.offScale, Math.round(v * 10) / 10 + 'px'); }
    }

    // targets
    if (!isSvg && el.matches('button, a[href], input, select, textarea, [role=button], [role=slider], [role=switch], [role=tab], [tabindex]:not([tabindex="-1"])')) {
      out.targets.push({sel: sel(el), w: Math.round(r.width), h: Math.round(r.height)});
    }

    // text
    let own = '';
    for (const n of el.childNodes) if (n.nodeType === 3) own += n.textContent;
    own = own.trim();
    if (!own) continue;
    let sc = 1;                                   // rendered scale (CSS transforms, SVG viewBox)
    if (isSvg) { try { sc = el.getScreenCTM().a || 1; } catch (e) {} }
    else if (el.offsetWidth > 0) sc = r.width / el.offsetWidth || 1;
    const css = parseFloat(cs.fontSize), weight = parseInt(cs.fontWeight) || 400;
    const size = css * sc;                        // size the person actually sees
    const family = cs.fontFamily.split(',')[0].replace(/["']/g, '').trim();
    bump(out.fontSizes, String(Math.round(size * 2) / 2)); bump(out.families, family); bump(out.weights, String(weight));
    let fg = parse(isSvg ? cs.fill : cs.color);
    if (!fg) continue;
    // effective background
    let layers = [], uncertain = false;
    for (let e = el; e; e = e.parentElement) {
      const s = getComputedStyle(e), c = parse(s.backgroundColor);
      if (s.backgroundImage && s.backgroundImage !== 'none') uncertain = true;
      if (c && c.a > 0) { layers.push(c); if (c.a >= 1) break; }
    }
    let bg = {r: 255, g: 255, b: 255, a: 1};
    for (let i = layers.length - 1; i >= 0; i--) bg = over(layers[i], bg);
    const fgc = over(fg, bg);
    const cr = ratio(fgc, bg);
    bump(out.textColors, hex(fgc));
    const large = size >= 24 || (size >= 18.66 && weight >= 700);
    const need = large ? 3 : 4.5;
    const rec = {sel: sel(el), text: own.slice(0, 28), size: Math.round(size * 10) / 10, weight, fg: hex(fgc), bg: hex(bg), ratio: Math.round(cr * 100) / 100, need, uncertain};
    if (cr < need) out.texts.push(rec);
    if (size < opts.minFont) out.small.push(rec);
  }

  // overflow
  const de = document.documentElement;
  out.overflow = {x: de.scrollWidth > de.clientWidth + 1, y: de.scrollHeight > de.clientHeight + 1,
                  sw: de.scrollWidth, cw: de.clientWidth, sh: de.scrollHeight, ch: de.clientHeight};

  // css feature checks
  let focus = false, reduced = false, motion = false;
  const walk = rules => { for (const r of rules) {
    if (r.cssRules) { if (r.conditionText && r.conditionText.includes('prefers-reduced-motion')) reduced = true; walk(r.cssRules); }
    if (r.selectorText && /:focus/.test(r.selectorText)) focus = true;
    if (r.style && ((r.style.transition && r.style.transition !== 'none') || (r.style.animationName && r.style.animationName !== 'none'))) motion = true;
    if (r.type === 7) motion = true; } };
  for (const sh of document.styleSheets) { try { walk(sh.cssRules); } catch (e) {} }
  out.css = {focusRule: focus, reducedMotionRule: reduced, usesMotion: motion};
  return out;
}
"""

def parse_viewports(s):
    vps = []
    for part in s.split(','):
        part = part.strip()
        scale = 1
        if '@' in part:
            part, sc = part.split('@')
            scale = float(sc)
        w, h = part.lower().split('x')
        vps.append((int(w), int(h), scale))
    return vps

def squint(src, dst_gray, dst_blur):
    try:
        from PIL import Image, ImageFilter, ImageOps
    except Exception:
        return False
    im = Image.open(src).convert('RGB')
    g = ImageOps.grayscale(im)
    g.save(dst_gray)
    g.filter(ImageFilter.GaussianBlur(radius=max(im.size) / 140)).save(dst_blur)
    return True

def top(d, n=8):
    return sorted(d.items(), key=lambda kv: -kv[1])[:n]

def main():
    if len(sys.argv) >= 4 and sys.argv[1] == 'contrast':
        sys.exit(contrast_cli(sys.argv[2], sys.argv[3]))
    ap = argparse.ArgumentParser()
    ap.add_argument('target')
    ap.add_argument('--out', default='design/audit')
    ap.add_argument('--viewports', default='1366x768@1,1920x1080@1,1440x900@2')
    ap.add_argument('--min-font', type=float, default=10)
    ap.add_argument('--min-target', type=int, default=24)
    ap.add_argument('--base', type=int, default=4)
    ap.add_argument('--allow-scroll', action='store_true', help='page-level scrolling is intentional (downgrade to info)')
    a = ap.parse_args()

    from playwright.sync_api import sync_playwright
    out = pathlib.Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    url = a.target if a.target.startswith(('http://', 'https://', 'file://')) else pathlib.Path(a.target).resolve().as_uri()

    findings, report = [], {'target': url, 'viewports': {}}
    def add(sev, vp, msg):
        findings.append({'severity': sev, 'viewport': vp, 'message': msg})

    with sync_playwright() as p:
        browser = p.chromium.launch()
        for (w, h, s) in parse_viewports(a.viewports):
            tag = f'{w}x{h}@{s:g}'
            page = browser.new_page(viewport={'width': w, 'height': h}, device_scale_factor=s)
            errs = []
            page.on('pageerror', lambda e: errs.append('pageerror: ' + str(e)))
            page.on('console', lambda m: errs.append('console.error: ' + m.text) if m.type == 'error' else None)
            page.on('requestfailed', lambda r: errs.append('request failed: ' + r.url[:120]))
            page.goto(url)
            page.wait_for_timeout(800)
            shot = out / f'shot-{tag}.png'
            page.screenshot(path=str(shot))
            sq = squint(shot, out / f'gray-{tag}.png', out / f'squint-{tag}.png')
            data = page.evaluate(PROBE, {'minFont': a.min_font, 'base': a.base})
            page.close()
            report['viewports'][tag] = data

            for e in errs:
                add('ERROR', tag, e)
            groups = {}
            for t in data['texts']:
                groups.setdefault(('c', t['sel'], t['fg'], t['bg'], t['ratio'], t['need'], t['uncertain']), []).append(t)
            for k, items in groups.items():
                _, sel, fg, bg, cr, need, unc = k
                ex = ', '.join('"%s"' % i['text'] for i in items[:3])
                add('WARN' if unc else 'ERROR', tag, f"{len(items)}x contrast {cr}:1 (needs {need}) {fg} on {bg} in {sel}, e.g. {ex}" + (' [background has image/gradient: verify by eye]' if unc else ''))
            groups = {}
            for t in data['small']:
                groups.setdefault((t['sel'], t['size']), []).append(t)
            for (sel, size), items in groups.items():
                ex = ', '.join('"%s"' % i['text'] for i in items[:3])
                add('ERROR', tag, f"{len(items)}x text renders at {size}px (floor {a.min_font:g}px) in {sel}, e.g. {ex}")
            small_targets = [t for t in data['targets'] if min(t['w'], t['h']) < a.min_target]
            if small_targets:
                ex = ', '.join(f"{t['sel']} {t['w']}x{t['h']}" for t in small_targets[:5])
                add('WARN', tag, f"{len(small_targets)} interactive element(s) under {a.min_target}px: {ex}")
            ov = data['overflow']
            if ov['x']:
                add('INFO' if a.allow_scroll else 'ERROR', tag, f"page overflows horizontally ({ov['sw']}px content in {ov['cw']}px window)")
            if ov['y']:
                add('INFO' if a.allow_scroll else 'WARN', tag, f"page overflows vertically ({ov['sh']}px content in {ov['ch']}px window)")
            if not sq:
                add('INFO', tag, 'Pillow not installed: grayscale/squint images skipped (pip install pillow)')

        browser.close()

    # token-discipline findings use the first viewport's data
    first = next(iter(report['viewports'].values()))
    sizes = len(first['fontSizes']); fams = len(first['families']); radii = len(first['radii']); cols = len(first['textColors'])
    if sizes > 8: add('WARN', 'all', f'{sizes} distinct font sizes (aim for 8 or fewer): {top(first["fontSizes"])}')
    if fams > 2: add('WARN', 'all', f'{fams} font families (aim for 2 or fewer): {top(first["families"])}')
    if radii > 5: add('WARN', 'all', f'{radii} distinct border radii (aim for 5 or fewer): {top(first["radii"])}')
    if cols > 9: add('WARN', 'all', f'{cols} distinct text colours (aim for 9 or fewer)')
    n = first['spacing'].get('n', 0)
    off = sum(first['offScale'].values())
    pct = (100.0 * off / n) if n else 0
    if pct > 15: add('WARN', 'all', f'{pct:.0f}% of padding/margin/gap values are off the {a.base}px scale: {top(first["offScale"])}')
    css = first['css']
    if not css['focusRule']: add('WARN', 'all', 'no :focus / :focus-visible rule found: keyboard focus may be invisible')
    if css['usesMotion'] and not css['reducedMotionRule']: add('WARN', 'all', 'uses transitions/animation but has no prefers-reduced-motion rule')

    summary = {
        'errors': sum(f['severity'] == 'ERROR' for f in findings),
        'warnings': sum(f['severity'] == 'WARN' for f in findings),
        'fontSizes': top(first['fontSizes']), 'families': top(first['families']), 'radii': top(first['radii']),
        'textColors': top(first['textColors'], 10), 'offScalePct': round(pct, 1), 'offScaleTop': top(first['offScale']),
    }
    report['summary'] = summary
    report['findings'] = findings
    (out / 'audit.json').write_text(json.dumps(report, indent=2))

    lines = [f"# UI audit: {url}", '',
             f"**Errors: {summary['errors']}   Warnings: {summary['warnings']}**   (errors block the Polish Gate)", '']
    for sev in ('ERROR', 'WARN', 'INFO'):
        items = [f for f in findings if f['severity'] == sev]
        if items:
            lines.append(f'## {sev}')
            seen = set()
            for f in items:
                key = (f['message'])
                if key in seen:
                    continue
                seen.add(key)
                lines.append(f"- [{f['viewport']}] {f['message']}")
            lines.append('')
    lines += ['## Token discipline (first viewport)',
              f"- Font sizes: {summary['fontSizes']}", f"- Families: {summary['families']}",
              f"- Radii: {summary['radii']}", f"- Text colours: {summary['textColors']}",
              f"- Off-scale spacing: {summary['offScalePct']}% {summary['offScaleTop']}", '',
              'Screenshots: ' + ', '.join(sorted(x.name for x in out.glob('*.png')))]
    (out / 'audit.md').write_text('\n'.join(lines) + '\n')
    print('\n'.join(lines))
    sys.exit(1 if summary['errors'] else 0)

if __name__ == '__main__':
    main()
