#!/usr/bin/env python3
"""Builds dist/Redline-Modular-standalone.html: ui/index.html with ui/engine.js inlined. One file, works offline."""
import pathlib
root = pathlib.Path(__file__).resolve().parents[1]
html = (root / 'ui' / 'index.html').read_text()
eng = (root / 'ui' / 'engine.js').read_text()
tag = '<script src="engine.js"></script>'
assert tag in html, 'index.html must include engine.js'
out = root / 'dist' / 'Redline-Modular-standalone.html'
out.parent.mkdir(exist_ok=True)
out.write_text(html.replace(tag, '<script>\n' + eng + '\n</script>'))
print('wrote', out, out.stat().st_size // 1024, 'KB')
