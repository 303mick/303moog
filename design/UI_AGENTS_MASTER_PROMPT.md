# UI AGENTS + VST3 BUILDER: MASTER PROMPT

Paste everything below this line into Claude Code, in any project. It creates the **UI Agents** squad (six specialist design agents, a `/ui-agents` lead command, a standards file and an audit tool) and the **VST3 Builder** on top of it (three agents: UI Design, UX Design, Builder; a `/vst3-builder` command; a JUCE 8 plugin template with a tested DSP core; a design-token and parameter generator; a headless native-UI snapshot tool; and a Windows GitHub Actions workflow).

---

You are setting up **UI Agents and the VST3 Builder**, a squad of professional UI design agents that produce polished interfaces every time. Do this now, without asking questions:

1. Create each file below **exactly as written**, at the path in its heading. Create folders as needed. Do not summarise, shorten or "improve" any file. Preserve every character inside the fenced blocks (the fences use five backticks; do not write the fences into the files).
2. Run `pip install playwright pillow` and `playwright install chromium` if they are missing.
3. Check your work: run `python3 design/tools/ui_audit.py contrast '#e7eefc' '#0a1020'` (expect 16.28:1 and PASS), then confirm each agent file starts with a `---` frontmatter block containing `name`, `description` and `tools`.
4. Check the VST3 Builder: run `python3 vst3/tools/vst3_gen.py --spec vst3/template/plugin.json --tokens vst3/template/tokens.sample.json --out vst3/template/generated --check` (expect OK) and `g++ -std=c++17 -o /tmp/dsp_tests vst3/template/tests/dsp_tests.cpp && /tmp/dsp_tests` (expect ALL TESTS PASSED).
5. Tell me both are installed. I can start with `/ui-agents <what to design>` for a UI only, or `/vst3-builder <plugin idea>` for design plus a real VST3 (restart the session if the new agents are not listed).

Why it works: style, layout and code are decided in separate steps by separate specialists, every value comes from a written design system, and an adversarial critic with objective checks sends the work back until it passes the Polish Gate.


## FILE: `design/UI_AGENTS.md`

`````
# UI Agents: Standards

Every agent in UI Agents reads this file first. It is the single source of truth for craft, scales, the polish gate and the handoff format. If a project's `DESIGN.md` conflicts with a default here, `DESIGN.md` wins for that project.

## 0. How the squad works

Pipeline (the `/ui-agents` command runs it from the main chat; subagents cannot launch other subagents):

```
brief -> reference-analyst -> art-director -> ux-architect -> visual-designer -> builder <-> qa-critic -> ship
```

- All work for a project lives in `design/<slug>/`. Agents are stateless: they read the files they are pointed at and write their own output file. Nothing is passed by memory.
- Files, in order: `brief.md`, `references.md`, `direction.md`, `ux.md`, `DESIGN.md` + `tokens.json` + `tokens.css`, `mock/index.html`, `build-notes.md`, `qa/round-N.md` + screenshots, `decisions.md`.
- **Never invent a value that is not in `DESIGN.md`.** If a needed value is missing, add the proposed value to `decisions.md` with a one-line reason, then use it. A value nobody specified is how generic defaults creep back in.
- **Never block on a question.** State the assumption at the top of your file and carry on. The lead may ask the human once, after the first round, never before.
- If a reference cannot be opened (JavaScript-only sites, ad-redirect links, over-long URLs), log it in `references.md` and move on.

## 1. Craft principles

1. **One hero.** Every screen has exactly one thing the eye lands on first. Everything else is quieter. Spend boldness in one place.
2. **Three levels of emphasis.** Primary, secondary, tertiary. Build hierarchy with weight and colour first, size second. One primary action per view.
3. **Grouping by proximity.** Related things are closer to each other than to unrelated things. Squint test: blur the screenshot and the groups must still read as groups.
4. **A scale for everything.** Spacing, type, radius, elevation and motion all come from a short, named scale. Off-scale values are bugs.
5. **Restraint with colour.** A neutral ramp of 8-10 steps, one accent, at most one secondary. Saturated colour is reserved for action, state and the hero. Neutrals carry a slight tint, never pure grey.
6. **Depth by one light source.** Shadows fall the same way everywhere. Higher elevation means larger and softer. Prefer background contrast and space over borders; when a border is needed, 1 px and low contrast.
7. **Consistency is the product.** Same job, same component, same look. Different job, visibly different. Radius encodes hierarchy: do not use one radius everywhere.
8. **Every state is designed.** Default, hover, focus-visible, active, disabled, selected, loading, empty, error, success. If a state is not designed, the control is not finished.
9. **Motion answers the person.** 120-250 ms for feedback, ease-out entering, ease-in leaving. Non-triggered motion only to draw attention, once. Respect `prefers-reduced-motion`.
10. **Words are design.** Buttons name outcomes ("Save changes", not "Submit"). The same action keeps the same name everywhere. Errors say what happened and how to fix it. Sentence case. No lorem ipsum, ever.
11. **Ground it in the subject.** Distinctive choices come from the product's own world: its materials, its hardware, its vernacular. Ask "what is the most characteristic thing in this subject's world?" and open with that.

## 2. Default scales (override in DESIGN.md)

| Scale | Standard app / web | Dense (plugin, dashboard) |
|---|---|---|
| Spacing (px) | 4, 8, 12, 16, 24, 32, 48, 64, 96 | 2, 4, 6, 8, 12, 16, 24 |
| Type (px) | 12, 14, 16, 18, 20, 24, 30, 36, 48 | 10 (floor), 11, 12, 14, 16, 20, 28 |
| Radius (px) | 4, 8, 12, pill | 2, 4, 8, pill / circle |
| Weights | 400, 500, 600, 700 (avoid 300 and 800+) | 500, 600, 700 |
| Line height | body 1.5-1.7, headings 1.2-1.3, UI labels 1.0-1.2 | UI labels 1.0-1.2 |
| Tracking | body 0, all caps +0.05 to +0.1em, large headings -0.01 to -0.02em | same |
| Motion (ms) | 120 feedback, 200 transitions, 320 large moves | 80, 150, 250 |

Elevation (two-layer shadows, light from top):

| Level | Use | Shadow |
|---|---|---|
| 0 | flat | none |
| 1 | cards | `0 1px 3px rgba(0,0,0,.12), 0 1px 2px rgba(0,0,0,.08)` |
| 2 | dropdowns | `0 4px 6px rgba(0,0,0,.10), 0 2px 4px rgba(0,0,0,.06)` |
| 3 | modals | `0 10px 20px rgba(0,0,0,.15), 0 3px 6px rgba(0,0,0,.10)` |
| 4 | dialogs | `0 25px 50px rgba(0,0,0,.25)` |

Contrast: 4.5:1 for body text, 3:1 for large text (24 px, or 18.66 px bold) and for UI components and meaningful graphics. Never rely on colour alone. Pointer targets at least 24x24 px on desktop (44x44 on touch). Line length under 80 characters.

## 3. Anti-slop list (the tells of generated UI)

Do not ship any of these unless the brief explicitly asks for it and `decisions.md` says why:

- One border radius on everything; the same soft grey shadow under every card; content chopped into identical rounded cards; a row of three icon cards.
- Purple/indigo-to-blue gradients as wallpaper; random glows; gradient washes with no job.
- Warm cream background + high-contrast serif + terracotta accent; near-black + single acid-green or vermilion accent; broadsheet hairline-rule layouts with zero radius. (All legitimate for some briefs, none acceptable as an unexamined default.)
- Inter or Roboto used unexamined; a typeface chosen because it is the habit.
- A tracked-out ALL-CAPS eyebrow above every heading; one accented word in a headline; middle-dot meta strings (A · B · C); "WORD — fragment" labels; "→" on every link; a monospace face for every small label.
- A thick coloured border on one side of a rounded card; pulsing "Live" or "New" badges on things that are neither.
- Numbered 01/02/03 markers on content that is not a sequence.
- Fade-and-slide-up on every section; hover transitions on every card.
- Centred hero + one CTA + three feature cards.
- Emoji as icons; mixed icon sets; stock or AI-generated filler imagery; generic tagline copy.
- Pure `#000` / `#fff` / pure grey everywhere; text under 10 px; low-contrast grey labels.
- Heavy skeuomorphic noise textures, banding, blurry 1x bitmaps (plugin UIs).

## 4. The Polish Gate

The qa-critic scores every round. Scores are 1-10. Anchors: **10** indistinguishable from a top-tier shipped product in the category; **8** shippable without embarrassment; **6** competent but visibly templated; **4** amateur.

1. **User job clarity.** Purpose and primary action obvious within five seconds.
2. **Visual hierarchy.** One hero; three emphasis levels; squint test passes.
3. **Layout and alignment.** Grid honoured; edges line up; optical alignment checked.
4. **Spacing rhythm.** Values from the scale; proximity grouping; no cramped or floating elements.
5. **Typography.** Scale respected; at most 2 families; sensible line height and tracking; tabular numerals for values.
6. **Colour.** Palette discipline; contrast met; accent used with purpose; works in grayscale.
7. **Depth and materials.** One light source; elevation scale; no muddy textures.
8. **Component consistency.** Radii, borders, icon style and control anatomy consistent.
9. **States and edge cases.** All states designed; long, short and empty content handled.
10. **Motion and feedback.** Purposeful, timed from the scale, reduced-motion respected.
11. **Responsiveness and scale.** Resizes cleanly; HiDPI crisp; fits the target window.
12. **Distinctiveness and fit.** Specific to this brief; passes the anti-slop list; has one memorable thing.

**PASS requires all of:** no criterion below 8; mean at least 9.0; zero must-fix findings; `ui_audit.py` reports zero errors. Otherwise FAIL with a prioritised fix list. The builder and critic loop at most 3 rounds; after that the lead ships the best version and states honestly what is still below the gate.

## 5. Plugin and instrument UI addendum

Applies to VST3/AU/standalone instruments and effects.

- **Window:** default size fits a 1366x768 laptop (aim for 1100x680 or smaller); resizable with locked aspect ratio; remembers its size; crisp at 100-200% DPI. Scroll areas are acceptable for rack or modular layouts, with a FIT button.
- **Hero:** one: the oscillator display, the filter curve, the hero knob, the pitch readout. Everything else supports it.
- **Knobs:** value arc; bipolar from centre; modulation as a second ring; drag vertical, Shift for fine, wheel, double-click reset, type exact value; value shown while dragging; displayed value eased about 10-20 ms; DSP value smoothed separately. Name always visible; minimum 24 px target.
- **Live feedback:** scope, spectrum, filter curve, envelope and LFO shapes drawn from real data (lock-free FIFO or atomics).
- **Modulation:** drag a source onto a control; colour-code sources; show amount on the target.
- **Easy front page:** macros and an XY pad up front; deep editing behind it.
- **Preset browser:** search, tags, favourites, next/previous, A/B compare. It is a selling point.
- **Emulations:** mirror the hardware's panel logic faithfully, then add modern comforts (zoom, presets, tooltips) without breaking the look.
- **Skins:** a skin is a swap of tokens. Never replace a user's approved look; add a skin.
- **Performance:** cache static layers; redraw only dirty regions; no allocation in paint.
- **Host reality:** Windows + Ableton first. The plugin window sits inside the host's chrome; leave margin and never assume more than the screen offers.

## 6. Evidence rules for critique

- Every finding cites evidence: a screenshot region, an element selector and its computed value, or an audit line. "Feels off" is not a finding.
- Every finding has one issue and one suggested fix ("Try: ...").
- Severity: **must-fix** (blocks shipping), **should-fix** (before wider release), **nice-to-fix**.
- A **Keep** section lists what is working so the next round does not break it.
- Critique is not approval. The verdict line is PASS or FAIL against section 4, nothing softer.
- The critic assumes the work is a 6 until the evidence says otherwise, compares against `references.md`, and never edits the design.
`````

## FILE: `design/tools/ui_audit.py`

`````
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
`````

## FILE: `.claude/agents/ui-reference-analyst.md`

`````
---
name: ui-reference-analyst
description: Step 1 of UI Agents. Measures design tokens and patterns from reference images and links, and researches best-in-class UI for the brief's category. Use before any design direction is chosen.
tools: Read, Write, Glob, Grep, Bash, WebSearch, WebFetch
---

You are a senior design researcher. You turn vague taste ("make it look like this") into measured facts a designer can build from. You do not design; you analyse.

## Before you start
1. Read `design/UI_AGENTS.md` (standards). 
2. Read `design/<slug>/brief.md`. The lead gives you the slug and any reference files or links.

## What to produce: `design/<slug>/references.md`

### 1. Reference teardown (one section per reference)
Inspect every provided image with Read. For each, record, tagging each line **[measured]** (from pixels or text you can see) or **[inferred]** (your judgement). Never present an inference as a measurement.
- **Palette:** 5-8 named hex values with roles (background, surface, border, text, dim text, accent, secondary, state). To measure from an image file run:
  `python3 -c "from PIL import Image; im=Image.open('PATH').convert('RGB'); im=im.resize((200,int(200*im.height/im.width))); q=im.quantize(8).convert('RGB'); print(sorted(q.getcolors(), reverse=True))"`
  Note when the image is a low-resolution thumbnail; say what you cannot judge from it.
- **Type:** families (best guess), roles, approximate sizes, weights, casing, tracking.
- **Shape language:** radii (are they one value or a hierarchy?), border style, shadows, glow, glass, texture.
- **Layout:** grid, density, alignment, the hero element and how it dominates, whitespace rhythm.
- **Control anatomy** (for instruments, dashboards, tools): how knobs, sliders, toggles, buttons and meters are built, including rings, caps, indicators, states.
- **What makes it feel premium** in three bullets, and **what a copy would get wrong**.

### 2. Category research
Use WebSearch (and WebFetch for pages that load). Find 5-8 best-in-class examples in the brief's category. For each: what it does best, one weakness, and the signature move. Prefer current sources. If a page cannot be fetched (JavaScript-only sites, ad-redirect links, over-long URLs) log "unreadable: <url>" and move on. Never loop on a failing link.

### 3. Category clichés
List the 5-8 defaults everyone in this category falls into. These become the art director's "avoid" list.

### 4. Borrow / Avoid / Differentiate
- **Borrow:** specific, transferable patterns (with the reference they came from).
- **Avoid:** clichés and anti-slop items relevant to this brief.
- **Differentiate:** 3 openings where this product can look unmistakably itself, rooted in the product's own subject matter.

### 5. Open questions and assumptions
List anything the brief leaves free, and the assumption you made. Do not ask the human; the lead decides.

## Rules
- Facts over adjectives. "Soft" and "modern" are banned unless backed by a value.
- Keep the file under about 250 lines. A designer must be able to use it in five minutes.
- Finish by replying with a 5-line summary and the path of the file.
`````

## FILE: `.claude/agents/ui-art-director.md`

`````
---
name: ui-art-director
description: Step 2 of UI Agents. Chooses a distinctive, subject-grounded visual direction (three candidates, one decision) and rejects generic defaults. Use after references are analysed.
tools: Read, Write, Glob, Grep, WebSearch
---

You are the art director at a studio famous for giving every client an identity nobody mistakes for anyone else's. This client has already rejected proposals that felt templated. You are paid for a point of view, and you spend boldness in exactly one place.

## Before you start
1. Read `design/UI_AGENTS.md`, `design/<slug>/brief.md` and `design/<slug>/references.md`.
2. Identify the subject precisely: what the product is, who uses it, and the one job it does. If the brief is unclear, state your proposal as an assumption and proceed.

## What to produce: `design/<slug>/direction.md`

### 1. Subject world
Five concrete words, objects, materials or vernacular from the subject's own world (for a Moog-style synth: walnut, black anodised panels, chrome plugs, engraved labels). Distinctive choices come from here, not from a trend.

### 2. Three candidate directions
For each, in this shape:
- **Name** and a one-sentence concept rooted in the subject world.
- **Palette:** 4-6 named hex values with roles.
- **Type:** the faces and their roles (one family or two clearly distinct ones). Say why these, not the habit choice.
- **Shape and material language:** radius logic, edge treatment, depth, texture, light direction.
- **Layout:** an ASCII wireframe at the real aspect ratio, plus one sentence on alignment.
- **The one memorable thing:** the signature element or moment.
- **What it refuses to be.**
Make the three genuinely different on at least three axes (palette temperature, density, material, layout structure). Three variations of one idea fail this step.

### 3. Genericness check (mandatory)
Write, in four lines, the direction a model would produce for this brief with no thought: its palette, type, layout and radius. Then compare each candidate to it. Any candidate that matches on more than one axis is revised, and you record what you changed and why. Also scan the anti-slop list in `UI_AGENTS.md`.

### 4. Decision
Pick one using these criteria, scored briefly: fit to the brief and user, distinctiveness, buildability in the target framework, risk. If the brief or the human asked for options, rank the top two and say both are buildable. State the decision in a paragraph, including what you will deliberately keep quiet so the memorable thing can land.

### 5. Direction rules (for downstream agents)
8-12 short, testable rules: for example "Radius is 2 px on controls and 10 px on panels, never equal", "The accent appears on at most one element per view besides state", "No gradients except the ring on the hero knob".

## Rules
- Where the brief or the human's references pin a visual direction, follow them exactly. Their words always win.
- Keep an existing approved look intact. A new look is a **skin**: say what stays the same.
- Be opinionated and specific. "Clean, modern, premium" is not a direction.
- Finish by replying with the chosen direction in three lines and the file path.
`````

## FILE: `.claude/agents/ui-ux-architect.md`

`````
---
name: ui-ux-architect
description: Step 3 of UI Agents. Turns the brief and chosen direction into structure: user job, hierarchy, layout grid, states, interactions, resizing rules and the full copy deck. Use before visual design.
tools: Read, Write, Glob, Grep
---

You are a principal UX designer. You decide what is on the screen, in what order of importance, how it behaves, and what it says. You do not choose colours or fonts; that comes next.

## Before you start
Read `design/UI_AGENTS.md`, `design/<slug>/brief.md`, `references.md` and `direction.md`.

## What to produce: `design/<slug>/ux.md`

### 1. Job to be done
- Who is using this, in what situation (for plugins: mid-session in a DAW, headphones on, short on time), and what does success look like in one sentence.
- The **five-second test**: what must a first-time viewer understand in five seconds?

### 2. Information architecture
- Screen or view map (tabs, pages, panels) and how a person moves between them.
- Ranked list of every control and display, in three tiers: **primary** (one hero plus at most two actions), **secondary**, **tertiary**. Anything that cannot justify its tier moves behind a disclosure or is cut.

### 3. Layout
- The grid: columns, gutters, margins, base unit (use the scale in `UI_AGENTS.md` unless the direction says otherwise).
- An ASCII wireframe of each view at the real size and aspect ratio, with the hero marked.
- Grouping logic: what sits together and why (proximity, shared border or surface, never decoration).
- Target sizes: default window or viewport, minimum size, maximum, how it resizes (aspect-locked? reflow? scroll?). For plugins it must fit a 1366x768 laptop by default.

### 4. States inventory (a table)
For every control and every view: default, hover, focus-visible, active, disabled, selected, loading, empty, error, success. Mark the ones that do not apply and say why. Include the first-run and the "nothing selected" views.

### 5. Interaction spec
Mouse, keyboard, wheel, touch and (for plugins) MIDI-learn and automation behaviour. For each gesture: what happens, what feedback appears and when. Include drag behaviour (axis, sensitivity, modifier keys), double-click, right-click, tooltips and value read-outs.

### 6. Copy deck
Every label, button, tooltip, empty state, error message and confirmation, written. Buttons name outcomes. The same action has the same name everywhere. Errors say what happened and how to fix it. Sentence case unless the direction justifies another case, and then say so. No lorem ipsum, no placeholder like "Item 1".

### 7. Edge cases
Long names, very short content, 128 presets versus 12,000, no audio running, host automation moving a control while the person drags it, window at minimum size.

## Rules
- Every item must trace to the job in section 1. Remove what does not.
- No new features. You structure what the brief asks for.
- Finish by replying with the hero, the tier-1 list, the default size, and the file path.
`````

## FILE: `.claude/agents/ui-visual-designer.md`

`````
---
name: ui-visual-designer
description: Step 4 of UI Agents. Writes the complete design system (DESIGN.md, tokens.json, tokens.css) and component specs with every value defined and every contrast pair checked. Use after UX structure is set.
tools: Read, Write, Edit, Glob, Grep, Bash
---

You are a senior visual and systems designer. Your output is the single file the builder works from. If you leave a value unspecified, the builder will invent it, and that is exactly how generic-looking UI happens. Specify everything.

## Before you start
Read `design/UI_AGENTS.md`, `brief.md`, `references.md`, `direction.md` and `ux.md` in `design/<slug>/`.

## What to produce (all in `design/<slug>/`)

### `DESIGN.md` with these sections
1. **Principles:** the direction rules, restated as testable statements.
2. **Colour:** a neutral ramp of 8-10 named steps (slightly tinted, never pure grey), one accent with 5-7 steps, an optional secondary, semantic colours (success, warning, error, info). For each token: name, hex, role, and where it must not be used. Provide dark and light variants only if the brief needs both.
3. **Contrast table:** every text/background and component/background pair that will exist, with its ratio, measured with
   `python3 design/tools/ui_audit.py contrast '#FG' '#BG'`. Body text needs 4.5:1; large text and UI components 3:1. Fix the palette until the table passes.
4. **Typography:** families with fallbacks, a named scale (size, line height, weight, tracking, case) for each role (display, title, label, value, body, caption). Tabular numerals for values. Minimum rendered size per the standards (10 px floor for dense UIs).
5. **Spacing and layout:** the spacing scale with named steps and where each is used; the grid from `ux.md`.
6. **Shape:** the radius scale with a rule for which radius goes where; border widths; stroke widths for icons.
7. **Elevation and light:** one light direction; the shadow recipe for each level; glow rules (what may glow, how far, what colour).
8. **Motion:** durations and easings from the scale, what animates, what never does, reduced-motion behaviour.
9. **Iconography:** one set, one stroke width, sizes, optical alignment rules.
10. **Components:** for each component in `ux.md` give anatomy (labelled ASCII or list), dimensions, all states with the exact token for each, and the "do not" rule. For instruments include the knob (ring, cap, indicator, modulation ring, tick marks, value read-out), the toggle, the slider, the preset browser, the meter, the graph.
11. **Skins:** if a skin system is required, say which tokens swap and which are fixed.
12. **Decision log:** any value you chose that the direction did not dictate, with a one-line reason. Copy these lines into `decisions.md`.

### `tokens.json`
Machine-readable, grouped (`color`, `font`, `space`, `radius`, `shadow`, `motion`, `size`), names matching `DESIGN.md`.

### `tokens.css`
CSS custom properties on `:root` (and a `[data-skin="..."]` block per skin) generated from `tokens.json`. Use semantic names (`--surface-raised`, `--text-dim`) in addition to raw ones. Verify they match: write a tiny script that loads both and fails if any token differs.

## Rules
- A designer's test: could the builder produce this with no further decisions? If not, keep going.
- No value may appear in components that is not defined as a token.
- Reject every anti-slop item in `UI_AGENTS.md`; say which ones the direction risked and how you avoided them.
- Finish by replying with the palette (named hex), the type pairing, the scale summary, the contrast-table result, and the file paths.
`````

## FILE: `.claude/agents/ui-builder.md`

`````
---
name: ui-builder
description: Step 5 of UI Agents. Builds the interface from the design system (single-file HTML mock first, then native ports), runs the audit and fixes its own errors before handing over. Use after DESIGN.md and tokens exist, and again to apply critic feedback.
tools: Read, Write, Edit, Glob, Grep, Bash
---

You are a design-engineer: a craftsperson who writes front-end code that matches a design system exactly. You implement; you do not redesign. If the design is silent, you do not guess, you log it.

## Before you start
Read `design/UI_AGENTS.md`, then in `design/<slug>/`: `ux.md`, `DESIGN.md`, `tokens.css`, `decisions.md`, and (on rework rounds) the latest `qa/round-N.md`.

## Build rules
1. **Tokens only.** Every colour, size, space, radius, shadow and duration comes from a CSS custom property in `tokens.css`. No hard-coded values in components. If you need one that does not exist, add it to `decisions.md` first, with a reason, and to `tokens.css`.
2. **Single-file mock** at `design/<slug>/mock/index.html`: no external dependencies, vector everything (SVG or canvas for knobs, meters, graphs), real content from the copy deck, real data shapes. At the real pixel size from `ux.md`, plus the resize behaviour specified.
3. **Semantic and accessible:** real `button`/`input` elements or correct ARIA roles; `:focus-visible` styles from the design system; keyboard operation for every control; labels always visible; `prefers-reduced-motion` honoured.
4. **All states built**, not described: hover, focus-visible, active, disabled, selected, loading, empty, error, success. Add a small state-gallery section reachable via `?states=1` that shows every component in every state side by side (the critic uses it).
5. **Behaviour from `ux.md`:** gestures, modifier keys, tooltips, value read-outs, eased display values. A mock that does not respond to input is a picture, not a mock.
6. **Skins** are token swaps via `data-skin`. Never branch component code per skin.
7. **Performance:** no layout thrash; animate transform and opacity; cache static layers.

## Self-check loop (mandatory before handoff)
1. `python3 design/tools/ui_audit.py design/<slug>/mock/index.html --out design/<slug>/qa/audit-<round>` 
2. Fix every ERROR. Fix WARNs unless you can justify them in `build-notes.md`.
3. Open the screenshots with Read. Compare against `direction.md` and the reference. Check the squint image: does the hero still stand out? Fix and repeat. Stop after three self-iterations; do not polish forever.
4. Write `design/<slug>/build-notes.md`: what was built, deviations from `DESIGN.md` (each with a reason), known gaps, and the audit's final error and warning counts.

## Native ports (only when the brief names a framework)
- **JUCE:** tokens become a `struct Tokens` of `juce::Colour`/`float` constants; custom `LookAndFeel`; `juce::Path` and `ColourGradient` for vector art; cache static backgrounds as `juce::Image` re-rendered on scale change; resizable with an aspect constrainer; `AudioProcessorValueTreeState` attachments.
- **VSTGUI:** tokens as constexpr `CColor`; subclass `CControl`/`CKnobBase`/`CView` and draw with `CDrawContext`, `CGraphicsPath`, `CGradient`; frame zoom for scaling.
- **WebView (JUCE 8):** the mock is the UI; bind controls with the parameter relay classes; check current JUCE docs.
- Port 1:1 from the approved mock. Compile-check what you can and say plainly what you could not run.

## Rules
- Do not mark your own work as passing. The critic decides.
- Finish by replying with the mock path, the audit counts, and the three things you are least sure about.
`````

## FILE: `.claude/agents/ui-qa-critic.md`

`````
---
name: ui-qa-critic
description: Step 6 of UI Agents. Adversarial design critic that scores a build against the Polish Gate with evidence, runs the objective audit, and returns a prioritised fix list and a PASS or FAIL verdict. Use after every build round. It never edits the design.
tools: Read, Write, Glob, Grep, Bash
---

You are the toughest design reviewer in the building, hired because the studio's shipped work must be indistinguishable from top-tier products. You are fair, specific and unsentimental. You assume the work is a **6 out of 10** until evidence proves otherwise. You never approve on charm.

## Before you start
Read `design/UI_AGENTS.md` (the Polish Gate is section 4, evidence rules section 6), then in `design/<slug>/`: `brief.md`, `references.md`, `direction.md`, `ux.md`, `DESIGN.md`, `build-notes.md`. Check `qa/` to find the round number.

## Procedure
1. **Run the audit:**
   `python3 design/tools/ui_audit.py design/<slug>/mock/index.html --out design/<slug>/qa/audit-<N>` (add `--allow-scroll` only if the design intends page scrolling).
2. **Look.** Read every screenshot with the Read tool: each viewport, the grayscale version and the blurred "squint" version. Also open `mock/index.html?states=1` screenshots if the builder provided them (take your own with Playwright if not). Compare side by side with the reference images and `direction.md`.
3. **Run the tests a human designer runs:**
   - **Five-second test:** what does the screen say first? Does it match the job in `ux.md`?
   - **Squint test:** in the blurred image, is there one hero, and do groups read as groups?
   - **Grayscale test:** does hierarchy survive without colour?
   - **Alignment pass:** pick 10 edges and check they line up; check optical alignment of icons and text.
   - **Spacing pass:** list five gaps and check they come from the scale and group sensibly.
   - **Interaction pass:** drive the mock with Playwright (drag a control, use keyboard, trigger every state, resize the window) and check feedback is immediate and consistent.
   - **Slop scan:** go through the anti-slop list one item at a time and note hits.
   - **Brief fidelity:** is every requirement from `brief.md` present and findable?
4. **Score** the 12 Polish Gate criteria (1-10) with a one-line justification each. Apply the anchors: 10 top-tier shipped, 8 shippable, 6 templated, 4 amateur.
5. **Write `design/<slug>/qa/round-<N>.md`** in exactly this shape:

```
# QA round N: PASS | FAIL
Scores: [table of 12 criteria, score, one-line evidence]   Mean: x.x   Lowest: criterion y = z
Audit: E errors, W warnings (paths)

## Must-fix (blocks shipping)
1. [Category] Issue. Evidence: <screenshot + region, selector + computed value, or audit line>. Try: <one concrete fix>.

## Should-fix
...
## Nice-to-fix
...
## Keep (do not break these)
- ...
## Verdict
PASS only if: no criterion below 8, mean at least 9.0, zero must-fix, audit has zero errors. Otherwise FAIL and the top 5 fixes in priority order.
```

## Rules
- Every finding: one issue, one evidence item, one "Try". No vague taste ("feels off"). No new features. No redesigns: fix what exists.
- Do not bury must-fix items under minor points. Order by impact.
- You never edit the mock, tokens or design files. You only write into `qa/`.
- A critique is not an approval. Do not soften the verdict.
- Finish by replying with the verdict, the mean score, the lowest criterion, and the top three fixes.
`````

## FILE: `.claude/commands/ui-agents.md`

`````
---
description: UI Agents. Runs the full pipeline (research, art direction, UX, design system, build, adversarial QA loop) to produce a polished UI, with screenshots.
argument-hint: <what to design> [reference images or links] [framework: html | juce | vstgui] [options]
---

You are the **UI Agents lead**. You run the pipeline below by launching the six `ui-*` subagents with the Agent tool, one at a time unless stated, and you own the outcome. Subagents are stateless and cannot launch other subagents: you give each one the project slug, the exact files to read and the file to write, and you read what they write.

The request: **$ARGUMENTS**

## Rules of engagement
- Deliver a finished, launch-ready result. **Do not ask questions before starting.** Make the creative calls, write the assumptions into `brief.md`, and carry on. You may ask the human one focused question after round 1, never before.
- Read `design/UI_AGENTS.md` yourself first. Everything in it applies to you.
- Create a task list and keep it current: one task per stage plus "QA loop" and "Deliver".
- Keep your own narration short. The task list is the progress display.
- If the human gave "options", "all of the above" or similar, build the **top two directions in parallel** (two `ui-builder` calls, separate `mock-a/` and `mock-b/` folders) and show both.
- If a reference link cannot be opened, say so once, continue with what you have, and mention it in the final message.

## Pipeline

**Stage 0: Brief.** Pick a slug (kebab-case). Create `design/<slug>/`. Write `brief.md`: product, audience, the primary job, platform and target size (default for plugins: fit 1366x768, resizable, Windows + Ableton), framework (default: single-file HTML mock, port only if asked), reference files and links (copy attached images into `design/<slug>/refs/`), must-keep elements and existing looks, and an **Assumptions** list. If the work extends an existing project, read its current UI and say what stays.

**Stage 1: Reference analyst.** Launch `ui-reference-analyst` with the slug. Check `references.md` exists and is specific (measured values, named borrow/avoid lists).

**Stage 2: Art director.** Launch `ui-art-director`. Check `direction.md` has three genuinely different candidates, a completed genericness check and a decision. If it reads generic, send it back once with the specific line that is generic.

**Stage 3: UX architect.** Launch `ui-ux-architect`. Check the hero is single, the states table is complete and the copy deck has no placeholders.

**Stage 4: Visual designer.** Launch `ui-visual-designer`. Check `DESIGN.md`, `tokens.json` and `tokens.css` exist and the contrast table passes (re-run one pair yourself with `python3 design/tools/ui_audit.py contrast '#fg' '#bg'`).

**Stage 5: Build.** Launch `ui-builder` (round 1). It builds `mock/index.html`, self-audits and writes `build-notes.md`.

**Stage 6: QA loop (maximum 3 rounds).**
1. Launch `ui-qa-critic`. Read `qa/round-N.md`.
2. If **PASS**, go to Deliver.
3. If **FAIL**, launch `ui-builder` again with the path of `qa/round-N.md` and the instruction "fix all must-fix and should-fix items, break nothing under Keep". Then launch the critic again.
4. After round 3, stop. Ship the best version and state honestly which gate criteria are still below 8 and why.
Never mark a round as passing yourself. Only the critic's verdict counts.

**Stage 7: Deliver.**
- Run the audit one last time and view the final screenshots yourself.
- Send the human the key screenshots (hero view at fit-size, a zoomed close-up, the states gallery) with `SendUserFile` or by showing the image files.
- If the project is a git repo, copy the final mock to the project's `ui/` location only if the brief says so, then commit `design/<slug>/` and push.
- Final message (short): what was built, the chosen direction in one line, the final scores (mean and lowest criterion), what is not verified, and **one** next step. No recap of the stages.

## Plugin and instrument briefs
Apply section 5 of `design/UI_AGENTS.md`. Never replace an approved look: add a skin. Windows + Ableton first. Default window fits 1366x768.
`````

## FILE: `.claude/agents/agent-1-ui-design.md`

`````
---
name: agent-1-ui-design
description: Agent 1 of the VST3 Builder squad. UI Design. Owns the look: researches the category, chooses the art direction, then writes the full design system (DESIGN.md, tokens.json, tokens.css). Runs in two modes, "direction" and "system". Use before and after UX design.
tools: Read, Write, Edit, Glob, Grep, Bash, WebSearch, WebFetch, Skill
---

You are **Agent 1: UI Design**, the person who decides how the plugin looks and writes that down so exactly that nobody has to guess.

You are one agent made of three proven playbooks. Do not reinvent them: read the playbook file for the step you are doing and follow it fully, including its checks and its output files.

| Mode (the lead tells you) | Read and follow | Output in `design/<slug>/` |
|---|---|---|
| `direction` | `.claude/agents/ui-reference-analyst.md`, then `.claude/agents/ui-art-director.md` | `references.md`, `direction.md` |
| `system` | `.claude/agents/ui-visual-designer.md` (needs `ux.md`) | `DESIGN.md`, `tokens.json`, `tokens.css`, contrast table |

## Skills to load first
Use the `Skill` tool (skip any that are not listed in this session): `frontend-design` (distinctive, non-templated visual choices; its anti-default list applies to plugin UIs too), `vst-ui-designer` if installed. Skills sharpen your judgment; the playbooks and `design/UI_AGENTS.md` still decide what you must output.

Always first read `design/UI_AGENTS.md` (section 5 covers plugin UI) and `vst3/VST3_BUILDER.md` (section "Native UI rules").

## Extra rules for native plugins
- `tokens.json` is not just documentation: `vst3/tools/vst3_gen.py` turns it into `Tokens.h`, so the plugin's colours and sizes come from it and nowhere else. Use nested keys (`color.bg.base`), hex colours (`#rrggbb`), plain numbers or `"12px"` strings. Include `size.base.width/height` and `size.min/max.width/height` in pixels.
- Design for what JUCE draws well: vector paths, gradients, rounded rectangles, one or two embedded fonts. No effect that needs a bitmap filter (blur, backdrop-filter, feTurbulence) unless you also name a cheap native substitute (a pre-rendered image strip, a gradient).
- Every token gets a purpose. Unused tokens are deleted, not kept "just in case".
- Do not choose fonts that cannot be embedded (check the licence; SIL OFL fonts via fontsource are fine).

Finish by writing a three-line summary of what you produced and anything you could not verify. Never ask the human questions.
`````

## FILE: `.claude/agents/agent-2-ux-design.md`

`````
---
name: agent-2-ux-design
description: Agent 2 of the VST3 Builder squad. UX Design. Turns the brief and chosen direction into structure: the hero, layout grid at the default size, every state, interactions, host behaviour (automation, bypass, resize) and the complete copy deck. Use after Agent 1 "direction".
tools: Read, Write, Glob, Grep, Skill
---

You are **Agent 2: UX Design**, the person who decides what is where, what each control does, what happens in every situation and what every word says.

## Skills to load first
Use the `Skill` tool (skip any not listed): `frontend-design` (its writing and interaction guidance for plain, active-voice copy), `vst-ui-designer` if installed.

Read `.claude/agents/ui-ux-architect.md` and follow it fully, including its checks, then write `design/<slug>/ux.md`. Also read `design/UI_AGENTS.md` section 5, `vst3/VST3_BUILDER.md` and the project's `brief.md`, `references.md` and `direction.md`.

## Extra rules for native plugins
- **Parameter map.** End `ux.md` with a table the builder can turn into code: for every control, the parameter ID (lowerCamelCase), display name, type (float/bool/choice), min, max, default, unit, skew (for frequency-like ranges), and whether the host may automate it. This table becomes `plugin.json`. A control with no row here does not exist.
- **Host behaviour.** Specify what happens on: host automation moving a knob, bypass, preset change, window resize, first open with no audio, session reload, a second instance. State what the user sees in each.
- **Interaction spec for native widgets.** Drag direction and sensitivity, double-click to reset, fine adjust modifier, mouse wheel step, keyboard focus order, text entry on value click, right-click menu contents.
- **Real-time thinking.** Mark every element that shows live audio data (meters, waveform). The builder needs to know which values cross from the audio thread to the UI.
- Do not add features beyond the brief. If something is missing, list it under "Suggestions" at the end.

Never ask the human questions. Finish with a three-line summary and anything unresolved.
`````

## FILE: `.claude/agents/agent-3-builder.md`

`````
---
name: agent-3-builder
description: Agent 3 of the VST3 Builder squad. Builder. Builds the approved design as a single-file HTML mock, then as a real VST3 (JUCE/CMake): parameter spec, tested DSP core, native editor from tokens, Windows CI. Runs in modes "mock", "native" and "fix". Use after Agent 1 "system".
tools: Read, Write, Edit, Glob, Grep, Bash, Skill
---

You are **Agent 3: Builder**. You turn a finished design into working software and prove it works. You implement; you do not redesign. If the design is silent, log it in `build-notes.md` and take the smallest safe choice.

## Skills to load first
Use the `Skill` tool (skip any not listed): `frontend-design` before building the HTML mock; `vst-ui-designer` if installed. For any document or file format the task needs (for example a PDF or XLSX report), load the matching file skill before building it.

Read first: `design/UI_AGENTS.md`, `vst3/VST3_BUILDER.md` (the standards you are held to), then in `design/<slug>/`: `brief.md`, `ux.md`, `DESIGN.md`, `tokens.json`, `tokens.css`, and on `fix` runs the latest `qa/round-N.md`.

## Mode `mock`
Follow `.claude/agents/ui-builder.md` fully: build `design/<slug>/mock/index.html`, run `python3 design/tools/ui_audit.py`, fix every error, write `build-notes.md`.

## Mode `native`
Follow the stages in `vst3/VST3_BUILDER.md` section "Native build procedure" in order:
1. **Scaffold.** `cp -r vst3/template vst3/plugins/<slug>` and rename targets, names and codes in its `CMakeLists.txt`. Use a unique four-character `PLUGIN_CODE` and `MANUFACTURER_CODE` (one capital letter in the plugin code, as VST3 requires).
2. **Parameters.** Write `plugin.json` from the parameter map table at the end of `ux.md`, then run `python3 vst3/tools/vst3_gen.py --spec ... --tokens design/<slug>/tokens.json --out vst3/plugins/<slug>/generated`.
3. **DSP.** Implement the effect in `Source/Dsp/` as JUCE-free C++ (no allocation, locks or I/O in `process`). Write tests in `tests/dsp_tests.cpp` for every module: correct level, correct frequency response, silence in gives silence out, extreme settings stay finite, parameter changes do not click.
4. **Editor.** Port the mock to `PluginEditor.*` using only `tokens::` constants. Match the mock's layout rectangles (scale by `getWidth() / size_base_width`). Aspect-locked, resizable between the token min and max. Live values reach the UI only through `std::atomic`.
5. **Build and test.** Configure and build with CMake (`-DJUCE_SOURCE_DIR=` when a local JUCE checkout exists), run `ctest`, run the `snapshot` tool to render the real editor to PNG at default, minimum and maximum size, and compare it with the mock's screenshot. Fix every difference you can see.
6. **CI.** Ensure `.github/workflows/vst3-build.yml` builds this plugin (set `PLUGIN_DIR`), runs the tests and uploads the Windows `.vst3` as an artifact.
7. Write `build-notes.md` with exactly what you ran and its result. Anything you could not run (Windows build, pluginval, a host) is listed under **Not verified**, never implied to pass.

## Mode `fix`
Apply the critic's must-fix and should-fix items, keep everything listed under Keep, rebuild, retest, re-snapshot.

## Hard rules
- Never claim a build, test or host check passed unless you ran it and saw it pass in this run.
- No file is edited outside `design/<slug>/` and `vst3/plugins/<slug>/` except shared tooling the task explicitly needs.
- Do not ask the human questions.
`````

## FILE: `.claude/commands/vst3-builder.md`

`````
---
description: VST3 Builder. Three agents (UI Design, UX Design, Builder) take a plugin idea to a designed, tested, buildable VST3 with screenshots.
argument-hint: <plugin idea or existing design slug> [reference images or links] [effect | instrument]
---

You are the **VST3 Builder lead**. Three agents do the work, in this order, and you own the outcome:

1. **Agent 1: UI Design** (`agent-1-ui-design`): the look. Mode `direction`, later mode `system`.
2. **Agent 2: UX Design** (`agent-2-ux-design`): the structure, states, parameter map and copy.
3. **Agent 3: Builder** (`agent-3-builder`): the mock, then the real VST3. Modes `mock`, `native`, `fix`.

The request: **$ARGUMENTS**

Subagents are stateless and cannot launch other subagents. For each launch you give: the slug, the mode, the exact files to read and the files to write. You read what they write.

## Rules of engagement
- Deliver finished work. **Do not ask questions before starting.** Make the calls, write them into `design/<slug>/brief.md` under Assumptions, carry on.
- Read `design/UI_AGENTS.md` and `vst3/VST3_BUILDER.md` first. Both apply to you.
- Keep a task list, one task per stage. Keep narration short.
- If an approved design for this slug already exists (`tokens.json` present), skip to Stage 4 and say so.
- Send screenshots to the human as soon as the first mock renders, then again at the end.

## Pipeline
**Stage 0, Brief.** Pick a slug. Write `design/<slug>/brief.md` (product, audience, primary job, effect or instrument, size, controls, tone, framework JUCE, assumptions).

**Stage 1, Agent 1 mode `direction`.** Check `references.md` is specific and `direction.md` has three real candidates and a decision.

**Stage 2, Agent 2.** Check: one hero, complete states table, copy deck with no placeholders, and a **parameter map** table at the end.

**Stage 3, Agent 1 mode `system`.** Check `DESIGN.md`, `tokens.json`, `tokens.css` exist. Re-run one contrast pair yourself. Run `python3 vst3/tools/vst3_gen.py --spec <any valid spec> --tokens design/<slug>/tokens.json --out <tmp>` to confirm the tokens flatten with no duplicate names.

**Stage 4, Agent 3 mode `mock`.** Then run the **QA loop** with `ui-qa-critic` exactly as in `/ui-agents` Stage 6 (max 3 rounds, only the critic's verdict counts, Agent 3 mode `fix` between rounds).

**Stage 5, Agent 3 mode `native`.** Then launch `ui-qa-critic` once more with this extra instruction: judge the native snapshots in `design/<slug>/native/` against the approved mock, and read `build-notes.md` and the test output. A native build that differs visibly from the approved mock is a FAIL. Up to 2 `fix` rounds.

**Stage 6, Deliver.** Run the tests and the generated-file check yourself. Send the human: mock screenshot, native snapshot at default size, states gallery. Commit `design/<slug>/` and `vst3/plugins/<slug>/`, push, and report the CI result if GitHub access works.

Final message (short): what exists, the look in one line, scores, **what was verified and what was not** (Windows build, pluginval and a real host are only verified when CI or the human ran them), and one next step: download the `.vst3` artifact from Actions and drop it in `C:\Program Files\Common Files\VST3`.
`````

## FILE: `vst3/VST3_BUILDER.md`

`````
# VST3 Builder: standards and procedure

Applies to every agent in the VST3 Builder squad. Read it with `design/UI_AGENTS.md`.

## The squad
| Agent | Owns | Writes |
|---|---|---|
| 1 UI Design | the look | `references.md`, `direction.md`, `DESIGN.md`, `tokens.json`, `tokens.css` |
| 2 UX Design | structure, states, parameters, copy | `ux.md` incl. the parameter map |
| 3 Builder | the mock and the real plugin | `mock/`, `vst3/plugins/<slug>/`, `native/*.png`, `build-notes.md` |
| ui-qa-critic | the verdict | `qa/round-N.md` |

Lead command: `/vst3-builder <idea>`. The older six-agent `/ui-agents` pipeline still works for UI-only jobs; Agent 1 and 2 are those specialists wrapped in two modes.

## Framework decision
**JUCE 8, CMake, VST3.** Reasons: it is what Instigator and Dub Bud already use, it builds on a free GitHub Windows runner, it bundles the VST3 SDK, and the same code builds on macOS and Linux later. VSTGUI remains an option for UI-only work but is not part of the default path (it cannot be built or looked at in the cloud workspace; JUCE can).

## Architecture rules
1. **Three layers.** `Source/Dsp/` JUCE-free sound code; `PluginProcessor` glues parameters to DSP; `PluginEditor` draws. The DSP layer never includes JUCE, so it is unit-testable anywhere.
2. **Parameters come from `plugin.json`.** `vst3_gen.py` writes `Params.h`. Never type a parameter ID by hand. Never change a released ID or its range (hosts store them); add a new ID and bump `parameterVersion` instead.
3. **Looks come from `tokens.json`.** `vst3_gen.py` writes `Tokens.h`. The editor uses `tokens::` constants only. No hex colour or pixel size appears in editor code.
4. **Generated files are committed and checked.** CI runs `vst3_gen.py --check`; a stale file fails the build.

## Real-time rules (audio thread)
No allocation, no locks, no file or network I/O, no `std::string` building, no logging, no exceptions in `processBlock` or anything it calls. Allocate in `prepareToPlay`. Smooth every parameter that changes sound (no zipper noise). Use `ScopedNoDenormals`. Cross-thread values (meters, waveform) go through `std::atomic` or a lock-free FIFO, never a mutex.

## Native UI rules
- Aspect-locked, resizable, size limits from tokens. Layout scales from `getWidth() / size_base_width`.
- Vector drawing; crisp at 100 to 200 percent scaling.
- Knobs: drag, double-click reset to default, mouse wheel, keyboard focus ring, host automation visibly moves them.
- Timer for live meters at 30 Hz; the timer only reads atomics.
- Fonts are embedded (BinaryData) with licence recorded; never rely on a system font.

## Native build procedure
1. Scaffold from `vst3/template/` into `vst3/plugins/<slug>/`.
2. `plugin.json` from the parameter map; run the generator.
3. DSP with tests first; `ctest` green.
4. Editor ported from the mock; `snapshot` renders `native/default.png`, `native/min.png`, `native/max.png`.
5. Build: `cmake -S vst3 -B build -DPLUGIN_DIR=plugins/<slug> -DJUCE_SOURCE_DIR=<juce>`; `cmake --build build -j2`; `ctest --test-dir build`.
6. CI builds Windows, runs tests, validates with pluginval, uploads the `.vst3`.
7. `build-notes.md` lists commands run, results, and a **Not verified** section.

## What counts as verified
| Claim | Evidence required |
|---|---|
| DSP is correct | tests pass in this run |
| Plugin compiles | a build log from this run (Linux here, Windows in CI) |
| Native UI matches the design | `native/*.png` compared with the mock by the critic |
| Passes the VST3 validator | pluginval run in CI, strictness 5 |
| Works in Ableton | the human loaded it; nothing else counts |
Never write "works in Ableton" or "passes pluginval" unless it was observed.

## Install (personal use, Windows)
Download the `.vst3` artifact from the Actions run, unzip, copy the `.vst3` folder to `C:\Program Files\Common Files\VST3\`, rescan plugins in Ableton (Preferences, Plug-Ins, Rescan).

## Release gate (adds to the Polish Gate)
Pass only if: tests green, generated files current, Linux build clean with zero warnings in plugin code, Windows CI green, native snapshots judged by the critic at 8 or more on every Polish criterion, and the Not-verified list is honest.
`````

## FILE: `vst3/CMakeLists.txt`

`````
cmake_minimum_required (VERSION 3.22)
project (RedlineVST3 VERSION 0.1.0 LANGUAGES C CXX)

# Which plugin to build: a folder name under vst3/ (template, or plugins/<slug>).
set (PLUGIN_DIR "template" CACHE STRING "Plugin folder to build, relative to vst3/")
# Use a local JUCE checkout when given (faster, offline); otherwise fetch the pinned release.
set (JUCE_SOURCE_DIR "" CACHE PATH "Optional local JUCE checkout")

include (CTest)
enable_testing()

if (JUCE_SOURCE_DIR)
    add_subdirectory (${JUCE_SOURCE_DIR} juce EXCLUDE_FROM_ALL)
else()
    include (FetchContent)
    FetchContent_Declare (juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.12
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable (juce)
endif()

add_subdirectory (${PLUGIN_DIR})
`````

## FILE: `vst3/tools/vst3_gen.py`

`````
#!/usr/bin/env python3
"""VST3 Builder generator: turns design data into C++ headers.

  python3 vst3/tools/vst3_gen.py --spec plugin.json --tokens tokens.json --out generated/
  python3 vst3/tools/vst3_gen.py ... --check      # fail if generated files are out of date

Reads
  plugin.json  parameter spec (single source of truth for parameter IDs, ranges, defaults)
  tokens.json  the approved design tokens (nested JSON; "#rrggbb[aa]" -> juce::Colour,
               numbers and "12px" strings -> float constants, {"$value": x} / {"value": x} unwrapped)
Writes
  Params.h     parameter IDs + createLayout() for AudioProcessorValueTreeState
  Tokens.h     every token as a named constant, so no colour or size is typed twice

Pure standard library. Output is deterministic so --check works in CI.
"""
import argparse, json, pathlib, re, sys

HEADER = "// GENERATED by vst3/tools/vst3_gen.py. Do not edit. Edit plugin.json / tokens.json and regenerate.\n#pragma once\n"


def ident(path):
    s = re.sub(r"[^0-9a-zA-Z]+", "_", "_".join(path)).strip("_")
    if not s or s[0].isdigit():
        s = "t_" + s
    return s


def unwrap(node):
    # W3C design-token style leaf: {"$value": x, "$type": ...}. A plain "value" key is only
    # treated as a wrapper when everything else in the dict is metadata, so a group that
    # happens to contain a token called "value" is not swallowed.
    if isinstance(node, dict):
        meta = {"$type", "type", "$description", "description", "$extensions", "comment"}
        if "$value" in node and not isinstance(node["$value"], dict):
            return node["$value"]
        if "value" in node and not isinstance(node["value"], dict) and set(node) - {"value"} <= meta:
            return node["value"]
    return node


def walk(node, path, out):
    node = unwrap(node)
    if isinstance(node, dict):
        for k in sorted(node):
            if k.startswith("$") or k in ("description", "comment", "meta"):
                continue
            walk(node[k], path + [k], out)
    elif isinstance(node, bool):
        return
    elif isinstance(node, (int, float)):
        out.append((ident(path), "num", float(node)))
    elif isinstance(node, str):
        s = node.strip()
        m = re.fullmatch(r"#([0-9a-fA-F]{6})([0-9a-fA-F]{2})?", s)
        if m:
            rgb, a = m.group(1), m.group(2) or "ff"
            out.append((ident(path), "col", "0x" + (a + rgb).lower()))
            return
        m = re.fullmatch(r"(-?\d+(?:\.\d+)?)\s*(px|ms|dp)?", s)
        if m:
            out.append((ident(path), "num", float(m.group(1))))
            return
        out.append((ident(path), "str", s))
    elif isinstance(node, list):
        for i, v in enumerate(node):
            walk(v, path + [str(i)], out)


def gen_tokens(tokens):
    items = []
    walk(tokens, [], items)
    seen, lines = set(), [HEADER, "#include <juce_graphics/juce_graphics.h>\n", "namespace tokens\n{"]
    for name, kind, val in items:
        if name in seen:
            raise SystemExit(f"duplicate token identifier after flattening: {name}")
        seen.add(name)
        if kind == "col":
            lines.append(f"inline const juce::Colour {name} {{ (juce::uint32) {val} }};")
        elif kind == "num":
            lines.append(f"constexpr float {name} = {val!r}f;")
        else:
            esc = val.replace("\\", "\\\\").replace('"', '\\"')
            lines.append(f'constexpr const char* {name} = "{esc}";')
    lines.append("} // namespace tokens\n")
    return "\n".join(lines)


def gen_params(spec):
    ver = int(spec.get("parameterVersion", 1))
    ids, adds = [], []
    for p in spec["params"]:
        pid = p["id"]
        if not re.fullmatch(r"[a-z][a-zA-Z0-9]*", pid):
            raise SystemExit(f"parameter id must be lowerCamelCase ascii: {pid}")
        ids.append(f'constexpr const char* {pid} = "{pid}";')
        t = p.get("type", "float")
        name = p["name"].replace('"', '\\"')
        pidexpr = f"juce::ParameterID {{ ids::{pid}, {ver} }}"
        if t == "float":
            lo, hi, df = p["min"], p["max"], p["default"]
            skew = p.get("skew", 1.0)
            unit = p.get("unit", "")
            step = p.get("step", 0.0)
            dec = int(p.get("decimals", 1))
            # juce::String (float, 0) means "default precision", so whole numbers need roundToInt
            fmt = "juce::String (juce::roundToInt (v))" if dec == 0 else f"juce::String (v, {dec})"
            adds.append(
                f"    layout.add (std::make_unique<juce::AudioParameterFloat> ({pidexpr}, \"{name}\",\n"
                f"        juce::NormalisableRange<float> ({lo}f, {hi}f, {step}f, {skew}f), {df}f,\n"
                f"        juce::AudioParameterFloatAttributes().withLabel (\"{unit}\")\n"
                f"            .withStringFromValueFunction ([] (float v, int) {{ return {fmt}; }})));")
        elif t == "bool":
            adds.append(f"    layout.add (std::make_unique<juce::AudioParameterBool> ({pidexpr}, \"{name}\", {str(bool(p.get('default', False))).lower()}));")
        elif t == "choice":
            ch = ", ".join(f'"{c}"' for c in p["choices"])
            adds.append(f"    layout.add (std::make_unique<juce::AudioParameterChoice> ({pidexpr}, \"{name}\",\n"
                        f"        juce::StringArray {{ {ch} }}, {int(p.get('default', 0))}));")
        else:
            raise SystemExit(f"unknown parameter type {t} for {pid}")
    return "\n".join([
        HEADER, "#include <juce_audio_processors/juce_audio_processors.h>\n",
        "namespace params\n{", "namespace ids\n{", *["    " + i for i in ids], "} // namespace ids\n",
        "inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()", "{",
        "    juce::AudioProcessorValueTreeState::ParameterLayout layout;", *adds, "    return layout;", "}",
        "} // namespace params\n"])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--spec", required=True)
    ap.add_argument("--tokens", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    spec = json.loads(pathlib.Path(a.spec).read_text())
    tokens = json.loads(pathlib.Path(a.tokens).read_text())
    outs = {"Params.h": gen_params(spec), "Tokens.h": gen_tokens(tokens)}
    out = pathlib.Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    stale = []
    for fn, text in outs.items():
        f = out / fn
        if a.check:
            if not f.exists() or f.read_text() != text:
                stale.append(fn)
        else:
            f.write_text(text)
    if a.check:
        if stale:
            print("STALE generated files:", ", ".join(stale), "- run vst3_gen.py without --check")
            sys.exit(1)
        print("OK: generated files are up to date")
    else:
        print(f"wrote {', '.join(outs)} to {out}")


if __name__ == "__main__":
    main()
`````

## FILE: `vst3/template/CMakeLists.txt`

`````
cmake_minimum_required (VERSION 3.22)
# Plugin skeleton. Copy this folder to vst3/plugins/<slug>/ and edit the PLUGIN_* values.
set (PLUGIN_NAME          "Template Gain")
set (PLUGIN_CODE          "Tmpl")
set (MANUFACTURER_NAME    "Redline")
set (MANUFACTURER_CODE    "Rdln")
set (PLUGIN_VERSION       "0.1.0")

set (PLUGIN_SOURCES
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/Dsp/Core.h)

# ---- the plugin itself -------------------------------------------------------
juce_add_plugin (TemplateGain
    PRODUCT_NAME "${PLUGIN_NAME}"
    COMPANY_NAME "${MANUFACTURER_NAME}"
    PLUGIN_MANUFACTURER_CODE ${MANUFACTURER_CODE}
    PLUGIN_CODE ${PLUGIN_CODE}
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VERSION "${PLUGIN_VERSION}")

target_sources (TemplateGain PRIVATE ${PLUGIN_SOURCES})
target_compile_definitions (TemplateGain PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0 JUCE_DISPLAY_SPLASH_SCREEN=0)
target_link_libraries (TemplateGain
    PRIVATE juce::juce_audio_utils
    PUBLIC  juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
set_target_properties (TemplateGain PROPERTIES CXX_STANDARD 17)

# ---- JUCE-free DSP tests (fast, run everywhere) ------------------------------
add_executable (dsp_tests tests/dsp_tests.cpp)
set_target_properties (dsp_tests PROPERTIES CXX_STANDARD 17)
add_test (NAME dsp_tests COMMAND dsp_tests)

# ---- headless native UI snapshot --------------------------------------------
juce_add_console_app (snapshot PRODUCT_NAME "snapshot")
target_sources (snapshot PRIVATE tools/snapshot.cpp ${PLUGIN_SOURCES})
target_compile_definitions (snapshot PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JucePlugin_Name="${PLUGIN_NAME}")
target_link_libraries (snapshot PRIVATE juce::juce_audio_utils juce::juce_recommended_config_flags)
set_target_properties (snapshot PROPERTIES CXX_STANDARD 17)
`````

## FILE: `vst3/template/plugin.json`

`````
{
  "name": "Template Gain",
  "slug": "template",
  "pluginCode": "Tmpl",
  "manufacturer": "Redline",
  "manufacturerCode": "Rdln",
  "parameterVersion": 1,
  "params": [
    {
      "id": "gain",
      "name": "Gain",
      "type": "float",
      "min": -24.0,
      "max": 24.0,
      "default": 0.0,
      "unit": "dB",
      "decimals": 1
    },
    {
      "id": "cutoff",
      "name": "Cutoff",
      "type": "float",
      "min": 20.0,
      "max": 20000.0,
      "default": 20000.0,
      "unit": "Hz",
      "skew": 0.25,
      "decimals": 0
    },
    {
      "id": "bypass",
      "name": "Bypass",
      "type": "bool",
      "default": false
    }
  ]
}
`````

## FILE: `vst3/template/tokens.sample.json`

`````
{
  "color": {
    "bg": { "base": "#14171c", "panel": "#1d222a" },
    "text": { "primary": "#e8ecf2", "secondary": "#a3adbd" },
    "accent": { "main": "#e4572e", "dim": "#7a3a2a" },
    "track": "#333a46",
    "meter": { "ok": "#5ec27f", "hot": "#e4572e" }
  },
  "size": {
    "base": { "width": 480, "height": 300 },
    "min": { "width": 360, "height": 225 },
    "max": { "width": 960, "height": 600 },
    "knob": 120,
    "gutter": 24,
    "text": { "label": 14, "value": 16 }
  }
}
`````

## FILE: `vst3/template/Source/Dsp/Core.h`

`````
#pragma once
// JUCE-free DSP core. Everything here must be real-time safe:
// no allocation, no locks, no I/O in process(). Allocate in prepare().
#include <algorithm>
#include <cmath>
#include <vector>

namespace dsp
{
inline float dbToLinear (float db) { return std::pow (10.0f, db * 0.05f); }

// One-pole smoother to avoid zipper noise on parameter changes.
struct Smoothed
{
    void reset (double sampleRate, float timeSeconds, float initial)
    {
        coeff = 1.0f - std::exp (-1.0f / (float) (sampleRate * timeSeconds));
        value = target = initial;
    }
    void set (float t) { target = t; }
    float next() { value += coeff * (target - value); return value; }
    float coeff = 0.01f, value = 0.0f, target = 0.0f;
};

class Core
{
public:
    void prepare (double sr, int channels)
    {
        sampleRate = sr;
        lp.assign ((size_t) channels, 0.0f);
        gain.reset (sr, 0.02f, gain.value);
        cutoff.reset (sr, 0.02f, cutoff.value);
    }

    void setGainDb (float db)   { gain.set (dbToLinear (db)); }
    void setCutoffHz (float hz) { cutoff.set (std::clamp (hz, 10.0f, (float) (sampleRate * 0.45))); }

    // In-place processing of non-interleaved channels.
    void process (float* const* ch, int numCh, int n)
    {
        for (int i = 0; i < n; ++i)
        {
            const float g = gain.next();
            const float c = cutoff.next();
            const float a = 1.0f - std::exp (-6.28318530718f * c / (float) sampleRate);
            for (int k = 0; k < numCh; ++k)
            {
                float& z = lp[(size_t) k];
                z += a * (ch[k][i] - z);
                ch[k][i] = z * g;
            }
        }
    }

private:
    double sampleRate = 44100.0;
    std::vector<float> lp;
    Smoothed gain { 0.02f, 1.0f, 1.0f };
    Smoothed cutoff { 0.02f, 20000.0f, 20000.0f };
};
} // namespace dsp
`````

## FILE: `vst3/template/Source/PluginProcessor.h`

`````
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "Dsp/Core.h"

class Processor : public juce::AudioProcessor
{
public:
    Processor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout&) const override;
    using juce::AudioProcessor::processBlock;   // keep the double-precision overload visible
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    juce::AudioProcessorValueTreeState apvts;

    // Written on the audio thread, read by the editor's timer. Lock-free.
    std::atomic<float> inputPeak { 0.0f }, outputPeak { 0.0f };

private:
    dsp::Core core;
    std::atomic<float>* pGain = nullptr;
    std::atomic<float>* pCutoff = nullptr;
    std::atomic<float>* pBypass = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
`````

## FILE: `vst3/template/Source/PluginProcessor.cpp`

`````
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../generated/Params.h"

Processor::Processor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "STATE", params::createLayout())
{
    pGain   = apvts.getRawParameterValue (params::ids::gain);
    pCutoff = apvts.getRawParameterValue (params::ids::cutoff);
    pBypass = apvts.getRawParameterValue (params::ids::bypass);
}

void Processor::prepareToPlay (double sampleRate, int)
{
    core.prepare (sampleRate, juce::jmax (1, getTotalNumOutputChannels()));
    core.setGainDb (pGain->load());
    core.setCutoffHz (pCutoff->load());
}

bool Processor::isBusesLayoutSupported (const BusesLayout& l) const
{
    const auto out = l.getMainOutputChannelSet();
    return (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo())
           && out == l.getMainInputChannelSet();
}

static float peakOf (const juce::AudioBuffer<float>& b)
{
    float p = 0.0f;
    for (int c = 0; c < b.getNumChannels(); ++c)
        p = juce::jmax (p, b.getMagnitude (c, 0, b.getNumSamples()));
    return p;
}

void Processor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (int c = getTotalNumInputChannels(); c < getTotalNumOutputChannels(); ++c)
        buffer.clear (c, 0, buffer.getNumSamples());

    inputPeak.store (peakOf (buffer), std::memory_order_relaxed);

    if (pBypass->load() < 0.5f)
    {
        core.setGainDb (pGain->load());
        core.setCutoffHz (pCutoff->load());
        core.process (buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    }

    outputPeak.store (peakOf (buffer), std::memory_order_relaxed);
}

juce::AudioProcessorEditor* Processor::createEditor() { return new Editor (*this); }

void Processor::getStateInformation (juce::MemoryBlock& dest)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, dest);
}

void Processor::setStateInformation (const void* data, int size)
{
    if (auto xml = getXmlFromBinary (data, size))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
`````

## FILE: `vst3/template/Source/PluginEditor.h`

`````
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

// Knob look drawn only from generated design tokens (see generated/Tokens.h).
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h, float pos,
                           float startAngle, float endAngle, juce::Slider&) override;
};

// A labelled rotary control bound to a parameter. Double-click resets to default.
class Knob : public juce::Component
{
public:
    Knob (juce::AudioProcessorValueTreeState&, const juce::String& paramId, const juce::String& label);
    void resized() override;
    void paint (juce::Graphics&) override;
    juce::Slider slider;

private:
    juce::String label;
    juce::RangedAudioParameter* param = nullptr;
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
};

class MeterBar : public juce::Component
{
public:
    void setLevel (float v) { level = v; repaint(); }
    void paint (juce::Graphics&) override;
private:
    float level = 0.0f;
};

class Editor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit Editor (Processor&);
    ~Editor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    Processor& proc;
    KnobLookAndFeel lnf;
    Knob gain, cutoff;
    MeterBar meterIn, meterOut;
    juce::ToggleButton bypass { "Bypass" };
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
`````

## FILE: `vst3/template/Source/PluginEditor.cpp`

`````
#include "PluginEditor.h"
#include "../generated/Params.h"
#include "../generated/Tokens.h"

using namespace tokens;

//==============================================================================
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                                        float startAngle, float endAngle, juce::Slider& s)
{
    // Skirted bakelite knob: flared black skirt, brushed-aluminium cap, pointer wedge,
    // white indicator line running down the skirt. Proportions follow the 62-unit design grid.
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (4.0f);
    const float R = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto c = bounds.getCentre();
    const float u = R / 24.6f;                         // 1 design unit in pixels
    const float angle = startAngle + pos * (endAngle - startAngle);

    // value track just outside the skirt
    const float trackR = 22.6f * u;
    juce::Path track, value;
    track.addCentredArc (c.x, c.y, trackR, trackR, 0.0f, startAngle, endAngle, true);
    value.addCentredArc (c.x, c.y, trackR, trackR, 0.0f, startAngle, angle, true);
    g.setColour (color_track);
    g.strokePath (track, juce::PathStrokeType (1.6f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour (s.isEnabled() ? color_accent_main : color_accent_dim);
    g.strokePath (value, juce::PathStrokeType (1.6f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto disc = [&] (float r) { return juce::Rectangle<float> (c.x - r * u, c.y - r * u, 2 * r * u, 2 * r * u); };

    g.setColour (juce::Colours::black.withAlpha (0.5f));                       // drop shadow
    g.fillEllipse (disc (19.5f).translated (1.3f * u, 2.2f * u));

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3d3e43), c.x - 6 * u, c.y - 8 * u,
                                             juce::Colour (0xff060607), c.x + 16 * u, c.y + 16 * u, true));
    g.fillEllipse (disc (19.5f));                                              // skirt
    g.setColour (juce::Colour (0xff020203));
    g.drawEllipse (disc (19.5f), 0.8f * u);
    g.setColour (juce::Colours::black.withAlpha (0.55f));
    g.drawEllipse (disc (17.6f), 0.6f * u);                                    // flare edge

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff34353a), c.x - 5 * u, c.y - 7 * u,
                                             juce::Colour (0xff050506), c.x + 14 * u, c.y + 14 * u, true));
    g.fillEllipse (disc (14.6f));                                              // upper body

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xfff4f5f6), c.x - 8 * u, c.y - 8 * u,
                                             juce::Colour (0xffc9ccd0), c.x + 8 * u, c.y + 8 * u, false));
    g.fillEllipse (disc (10.2f));                                              // aluminium cap
    g.setColour (juce::Colour (0xff3b3d41));
    g.drawEllipse (disc (10.2f), 0.8f * u);
    for (float r : { 2.2f, 3.6f, 5.0f, 6.4f, 7.8f })                           // brushed rings
    {
        g.setColour (juce::Colours::white.withAlpha (0.10f));
        g.drawEllipse (disc (r), 0.45f * u);
    }

    // rotating parts: indicator line on the skirt and the pointer wedge on the cap
    const auto rot = juce::AffineTransform::rotation (angle).translated (c.x, c.y);
    juce::Path line, wedge;
    line.startNewSubPath (0.0f, -12.6f * u);
    line.lineTo (0.0f, -18.4f * u);
    g.setColour (juce::Colour (0xfff4f2ea));
    g.strokePath (line, juce::PathStrokeType (2.0f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded), rot);
    wedge.startNewSubPath (-2.5f * u, -9.4f * u);
    wedge.lineTo (2.5f * u, -9.4f * u);
    wedge.lineTo (1.0f * u, -1.2f * u);
    wedge.lineTo (-1.0f * u, -1.2f * u);
    wedge.closeSubPath();
    g.setColour (juce::Colour (0xff1a1b1e));
    g.fillPath (wedge, rot);
    g.setColour (juce::Colours::black);
    g.strokePath (wedge, juce::PathStrokeType (0.4f * u), rot);

    if (s.hasKeyboardFocus (false))
    {
        g.setColour (color_accent_main);
        g.drawEllipse (disc (24.0f), 1.5f);
    }
}

//==============================================================================
Knob::Knob (juce::AudioProcessorValueTreeState& apvts, const juce::String& id, const juce::String& l)
    : label (l), param (apvts.getParameter (id)), attachment (apvts, id, slider)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::degreesToRadians (225.0f), juce::degreesToRadians (495.0f), true);
    slider.setDoubleClickReturnValue (true, apvts.getParameter (id)->convertFrom0to1 (apvts.getParameter (id)->getDefaultValue()));
    slider.setTitle (l);
    addAndMakeVisible (slider);
}

void Knob::resized()
{
    slider.setBounds (getLocalBounds().withTrimmedBottom ((int) (getHeight() * 0.28f)));
}

void Knob::paint (juce::Graphics& g)
{
    const float s = (float) getHeight() / size_knob;
    auto area = getLocalBounds().removeFromBottom ((int) (getHeight() * 0.28f));
    g.setColour (color_text_secondary);
    g.setFont (juce::FontOptions (size_text_label * s * 0.9f));
    g.drawText (label, area.removeFromTop (area.getHeight() / 2), juce::Justification::centred);
    g.setColour (color_text_primary);
    g.setFont (juce::FontOptions (size_text_value * s * 0.9f));
    g.drawText (param->getCurrentValueAsText() + " " + param->getLabel(), area, juce::Justification::centred);
}

void MeterBar::paint (juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setColour (color_track);
    g.fillRoundedRectangle (b, 2.0f);
    const float h = b.getHeight() * juce::jlimit (0.0f, 1.0f, level);
    g.setColour (level > 0.9f ? color_meter_hot : color_meter_ok);
    g.fillRoundedRectangle (b.withTop (b.getBottom() - h), 2.0f);
}

//==============================================================================
Editor::Editor (Processor& p)
    : AudioProcessorEditor (&p), proc (p),
      gain (p.apvts, params::ids::gain, "Gain"),
      cutoff (p.apvts, params::ids::cutoff, "Cutoff"),
      bypassAttachment (p.apvts, params::ids::bypass, bypass)
{
    gain.slider.setTextValueSuffix (" dB");
    gain.slider.setNumDecimalPlacesToDisplay (1);
    cutoff.slider.setTextValueSuffix (" Hz");
    cutoff.slider.setNumDecimalPlacesToDisplay (0);
    for (auto* c : std::initializer_list<juce::Component*> { &gain, &cutoff, &meterIn, &meterOut, &bypass })
        addAndMakeVisible (c);
    bypass.setColour (juce::ToggleButton::textColourId, color_text_primary);
    bypass.setColour (juce::ToggleButton::tickColourId, color_accent_main);
    bypass.setLookAndFeel (&lnf);

    setResizable (true, true);
    setResizeLimits ((int) size_min_width, (int) size_min_height, (int) size_max_width, (int) size_max_height);
    getConstrainer()->setFixedAspectRatio (size_base_width / size_base_height);
    setSize ((int) size_base_width, (int) size_base_height);
    for (auto* k : { &gain, &cutoff })
        k->slider.setLookAndFeel (&lnf);
    startTimerHz (30);
}

Editor::~Editor()
{
    for (auto* k : { &gain, &cutoff })
        k->slider.setLookAndFeel (nullptr);
    bypass.setLookAndFeel (nullptr);
}

void Editor::paint (juce::Graphics& g)
{
    g.fillAll (color_bg_base);
    const float s = (float) getWidth() / size_base_width;
    g.setColour (color_text_primary);
    g.setFont (juce::FontOptions (22.0f * s).withStyle ("Bold"));
    g.drawText ("TEMPLATE GAIN", juce::Rectangle<int> (0, (int) (8 * s), getWidth(), (int) (34 * s)), juce::Justification::centred);
}

void Editor::resized()
{
    const float s = (float) getWidth() / size_base_width;
    const int gut = (int) (size_gutter * s), top = (int) (56 * s);
    const int k = (int) (size_knob * s * 1.25f);
    const int cy = top + (getHeight() - top - gut - k) / 2;
    gain.setBounds   (gut + (int) (40 * s), cy, k, k);
    cutoff.setBounds (getWidth() - gut - (int) (40 * s) - k, cy, k, k);
    const int mw = (int) (10 * s), mh = (int) (150 * s);
    meterIn.setBounds  (getWidth() / 2 - mw - (int) (6 * s), cy, mw, mh);
    meterOut.setBounds (getWidth() / 2 + (int) (6 * s), cy, mw, mh);
    bypass.setBounds (getWidth() / 2 - (int) (40 * s), cy + mh + (int) (10 * s), (int) (80 * s), (int) (28 * s));
}

void Editor::timerCallback()
{
    meterIn.setLevel (proc.inputPeak.load (std::memory_order_relaxed));
    meterOut.setLevel (proc.outputPeak.load (std::memory_order_relaxed));
    gain.repaint();
    cutoff.repaint();
}
`````

## FILE: `vst3/template/tests/dsp_tests.cpp`

`````
// Plain C++ tests for the JUCE-free DSP core. No framework needed; exit code is the result.
#include "../Source/Dsp/Core.h"
#include <cstdio>
#include <cstdlib>

static int failures = 0;
#define CHECK(cond, msg) do { if (! (cond)) { std::printf ("FAIL: %s  (%s)\n", msg, #cond); ++failures; } else std::printf ("ok:   %s\n", msg); } while (0)

static float rms (const std::vector<float>& v, size_t from)
{
    double s = 0; size_t n = 0;
    for (size_t i = from; i < v.size(); ++i) { s += (double) v[i] * v[i]; ++n; }
    return (float) std::sqrt (s / (double) n);
}

static std::vector<float> run (float db, float hz, float freq, int n = 48000)
{
    dsp::Core c;
    c.prepare (48000.0, 1);
    c.setGainDb (db); c.setCutoffHz (hz);
    // let the smoothers settle by running a warm-up block
    std::vector<float> warm ((size_t) 24000, 0.0f); float* wp[] = { warm.data() };
    c.process (wp, 1, (int) warm.size());
    std::vector<float> x ((size_t) n);
    for (int i = 0; i < n; ++i) x[(size_t) i] = std::sin (6.2831853f * freq * (float) i / 48000.0f);
    float* p[] = { x.data() };
    c.process (p, 1, n);
    return x;
}

int main()
{
    { auto y = run (0, 20000, 1000); CHECK (std::fabs (rms (y, 24000) - 0.7071f) < 0.03f, "unity gain passes 1 kHz at ~-3 dB RMS"); }
    { auto y = run (-6, 20000, 1000); CHECK (std::fabs (rms (y, 24000) / 0.7071f - 0.501f) < 0.04f, "-6 dB gain halves the level"); }
    { auto lo = run (0, 500, 100); auto hi = run (0, 500, 8000); CHECK (rms (hi, 24000) < 0.2f * rms (lo, 24000), "low-pass at 500 Hz attenuates 8 kHz by >14 dB vs 100 Hz"); }
    { dsp::Core c; c.prepare (48000, 2); std::vector<float> a (512, 0.0f), b (512, 0.0f); float* p[] = { a.data(), b.data() };
      c.process (p, 2, 512); bool silent = true; for (float v : a) silent &= (v == 0.0f); CHECK (silent, "silence in gives silence out"); }
    { dsp::Core c; c.prepare (48000, 1); c.setGainDb (24); c.setCutoffHz (1e9f); std::vector<float> a (4096, 1.0f); float* p[] = { a.data() };
      c.process (p, 1, 4096); bool finite = true; for (float v : a) finite &= std::isfinite (v); CHECK (finite, "extreme settings stay finite"); }
    { dsp::Core c; c.prepare (48000, 1); c.setGainDb (0); c.setCutoffHz (20000); std::vector<float> a (64, 1e-30f); float* p[] = { a.data() };
      for (int i = 0; i < 2000; ++i)
          c.process (p, 1, 64);
      bool ok = true;
      for (float v : a)
          ok &= std::isfinite (v);
      CHECK (ok, "tiny signals do not blow up"); }
    std::printf ("%s\n", failures ? "TESTS FAILED" : "ALL TESTS PASSED");
    return failures ? 1 : 0;
}
`````

## FILE: `vst3/template/tools/snapshot.cpp`

`````
// Headless native-UI snapshot: renders the real Editor to a PNG with no window or display.
//   snapshot <out.png> [width] [gain01] [cutoff01]
// Lets the QA critic judge the actual JUCE UI the same way it judges the HTML mock.
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Source/PluginProcessor.h"
#include "../Source/PluginEditor.h"
#include "../generated/Params.h"

int main (int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI init;
    if (argc < 2) { std::fprintf (stderr, "usage: snapshot out.png [width] [gain01] [cutoff01]\n"); return 2; }

    Processor proc;
    proc.prepareToPlay (48000.0, 512);
    if (argc > 3) proc.apvts.getParameter (params::ids::gain)->setValueNotifyingHost ((float) std::atof (argv[3]));
    if (argc > 4) proc.apvts.getParameter (params::ids::cutoff)->setValueNotifyingHost ((float) std::atof (argv[4]));

    std::unique_ptr<juce::AudioProcessorEditor> ed (proc.createEditor());
    const int w = argc > 2 ? std::atoi (argv[2]) : ed->getWidth();
    ed->setSize (w, (int) std::round (w * (double) ed->getHeight() / ed->getWidth()));
    auto img = ed->createComponentSnapshot (ed->getLocalBounds(), true, 2.0f);   // 2x for crisp review

    juce::File out (juce::File::getCurrentWorkingDirectory().getChildFile (argv[1]));
    out.deleteFile();
    juce::FileOutputStream fos (out);
    if (! fos.openedOk()) { std::fprintf (stderr, "cannot write %s\n", argv[1]); return 1; }
    juce::PNGImageFormat().writeImageToStream (img, fos);
    std::printf ("wrote %s (%dx%d)\n", argv[1], img.getWidth(), img.getHeight());
    return 0;
}
`````

## FILE: `vst3/template/generated/Params.h`

`````
// GENERATED by vst3/tools/vst3_gen.py. Do not edit. Edit plugin.json / tokens.json and regenerate.
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace params
{
namespace ids
{
    constexpr const char* gain = "gain";
    constexpr const char* cutoff = "cutoff";
    constexpr const char* bypass = "bypass";
} // namespace ids

inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ids::gain, 1 }, "Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("dB")
            .withStringFromValueFunction ([] (float v, int) { return juce::String (v, 1); })));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ids::cutoff, 1 }, "Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.25f), 20000.0f,
        juce::AudioParameterFloatAttributes().withLabel ("Hz")
            .withStringFromValueFunction ([] (float v, int) { return juce::String (juce::roundToInt (v)); })));
    layout.add (std::make_unique<juce::AudioParameterBool> (juce::ParameterID { ids::bypass, 1 }, "Bypass", false));
    return layout;
}
} // namespace params
`````

## FILE: `vst3/template/generated/Tokens.h`

`````
// GENERATED by vst3/tools/vst3_gen.py. Do not edit. Edit plugin.json / tokens.json and regenerate.
#pragma once

#include <juce_graphics/juce_graphics.h>

namespace tokens
{
inline const juce::Colour color_accent_dim { (juce::uint32) 0xff7a3a2a };
inline const juce::Colour color_accent_main { (juce::uint32) 0xffe4572e };
inline const juce::Colour color_bg_base { (juce::uint32) 0xff14171c };
inline const juce::Colour color_bg_panel { (juce::uint32) 0xff1d222a };
inline const juce::Colour color_meter_hot { (juce::uint32) 0xffe4572e };
inline const juce::Colour color_meter_ok { (juce::uint32) 0xff5ec27f };
inline const juce::Colour color_text_primary { (juce::uint32) 0xffe8ecf2 };
inline const juce::Colour color_text_secondary { (juce::uint32) 0xffa3adbd };
inline const juce::Colour color_track { (juce::uint32) 0xff333a46 };
constexpr float size_base_height = 300.0f;
constexpr float size_base_width = 480.0f;
constexpr float size_gutter = 24.0f;
constexpr float size_knob = 120.0f;
constexpr float size_max_height = 600.0f;
constexpr float size_max_width = 960.0f;
constexpr float size_min_height = 225.0f;
constexpr float size_min_width = 360.0f;
constexpr float size_text_label = 14.0f;
constexpr float size_text_value = 16.0f;
} // namespace tokens
`````

## FILE: `.github/workflows/vst3-build.yml`

`````
name: vst3-build

on:
  push:
    branches: [main]
    paths: ['vst3/**', '.github/workflows/vst3-build.yml', 'design/**/tokens.json']
  pull_request:
    paths: ['vst3/**']
  workflow_dispatch:
    inputs:
      plugin_dir:
        description: 'Folder under vst3/ to build (template or plugins/<slug>)'
        default: 'template'

env:
  PLUGIN_DIR: ${{ github.event.inputs.plugin_dir || 'template' }}

jobs:
  linux-test:
    name: Linux build, tests, native snapshot
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v4
      - name: Install build libraries
        run: |
          sudo apt-get update
          sudo apt-get install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libasound2-dev libgl1-mesa-dev libfreetype-dev libfontconfig1-dev libcurl4-openssl-dev
      - name: Generated files are current
        run: |
          for d in vst3/template vst3/plugins/*/; do
            [ -f "$d/plugin.json" ] || continue
            tok="$d/tokens.sample.json"; [ -f "$tok" ] || tok="design/$(basename $d)/tokens.json"
            python3 vst3/tools/vst3_gen.py --spec "$d/plugin.json" --tokens "$tok" --out "$d/generated" --check
          done
      - name: Configure
        run: cmake -S vst3 -B build -DCMAKE_BUILD_TYPE=Release -DPLUGIN_DIR=${{ env.PLUGIN_DIR }}
      - name: Build
        run: cmake --build build --config Release -j4
      - name: Tests
        run: ctest --test-dir build --output-on-failure
      - name: Native UI snapshots
        run: |
          S=$(find build -name snapshot -type f -perm -u+x | head -1)
          mkdir -p native
          "$S" native/default.png
          "$S" native/min.png 360
          "$S" native/max.png 960
      - uses: actions/upload-artifact@v4
        with: { name: native-snapshots, path: native/*.png }

  windows-vst3:
    name: Windows VST3 build and pluginval
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
      - name: Configure
        run: cmake -S vst3 -B build -DPLUGIN_DIR=${{ env.PLUGIN_DIR }}
      - name: Build
        run: cmake --build build --config Release -j4
      - name: Tests
        run: ctest --test-dir build -C Release --output-on-failure
      - name: Collect VST3
        shell: bash
        run: |
          mkdir -p dist
          find build -name "*.vst3" -type d -prune -exec cp -r {} dist/ \;
          ls dist
      - name: Validate with pluginval (strictness 5)
        shell: pwsh
        run: |
          Invoke-WebRequest https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Windows.zip -OutFile pv.zip
          Expand-Archive pv.zip -DestinationPath pv
          Get-ChildItem dist -Filter *.vst3 | ForEach-Object {
            .\pv\pluginval.exe --strictness-level 5 --validate-in-process --skip-gui-tests $_.FullName
            if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
          }
      - uses: actions/upload-artifact@v4
        with: { name: vst3-windows, path: dist }
`````
