#!/usr/bin/env python3
"""Regenerates design/UI_AGENTS_MASTER_PROMPT.md from the real UI Agents files.

Run from the repo root:  python3 design/tools/build_master_prompt.py
Also runs a round-trip check: every embedded file must match the file on disk.
"""
import pathlib, re, sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
FILES = [
    'design/UI_AGENTS.md',
    'design/tools/ui_audit.py',
    '.claude/agents/ui-reference-analyst.md',
    '.claude/agents/ui-art-director.md',
    '.claude/agents/ui-ux-architect.md',
    '.claude/agents/ui-visual-designer.md',
    '.claude/agents/ui-builder.md',
    '.claude/agents/ui-qa-critic.md',
    '.claude/commands/ui-agents.md',
]
FENCE = '`' * 5   # five backticks, so files containing ``` blocks embed safely

HEADER = """# UI AGENTS: MASTER PROMPT

Paste everything below this line into Claude Code, in any project. It creates the **UI Agents** squad: six specialist design agents, a `/ui-agents` lead command, a standards file and an audit tool.

---

You are setting up **UI Agents**, a squad of professional UI design agents that produce polished interfaces every time. Do this now, without asking questions:

1. Create each file below **exactly as written**, at the path in its heading. Create folders as needed. Do not summarise, shorten or "improve" any file. Preserve every character inside the fenced blocks (the fences use five backticks; do not write the fences into the files).
2. Run `pip install playwright pillow` and `playwright install chromium` if they are missing.
3. Check your work: run `python3 design/tools/ui_audit.py contrast '#e7eefc' '#0a1020'` (expect 16.28:1 and PASS), then confirm each agent file starts with a `---` frontmatter block containing `name`, `description` and `tools`.
4. Tell me the agents are installed, and that I can start with `/ui-agents <what to design>` (restart the session if the new agents are not listed).

Why it works: style, layout and code are decided in separate steps by separate specialists, every value comes from a written design system, and an adversarial critic with objective checks sends the work back until it passes the Polish Gate.

"""

def build():
    parts = [HEADER]
    for rel in FILES:
        text = (ROOT / rel).read_text()
        assert FENCE not in text, rel
        parts.append(f"## FILE: `{rel}`\n\n{FENCE}\n{text.rstrip(chr(10))}\n{FENCE}\n")
    out = ROOT / 'design' / 'UI_AGENTS_MASTER_PROMPT.md'
    out.write_text('\n'.join(parts))
    return out

def verify(path):
    md = path.read_text()
    pat = re.compile(r"## FILE: `([^`]+)`\n\n" + FENCE + r"\n(.*?)\n" + FENCE + r"\n", re.S)
    found = {m.group(1): m.group(2) for m in pat.finditer(md)}
    bad = [rel for rel in FILES if found.get(rel) != (ROOT / rel).read_text().rstrip('\n')]
    missing = [rel for rel in FILES if rel not in found]
    if bad or missing:
        print('MISMATCH', bad, 'MISSING', missing)
        sys.exit(1)
    print(f'OK: {len(FILES)} files embedded and verified byte-for-byte ({path.stat().st_size // 1024} KB)')

if __name__ == '__main__':
    verify(build())
