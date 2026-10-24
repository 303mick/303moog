# UI Agents

Six specialist design agents plus a lead command that produce polished UI every time, by separating the decisions that make AI interfaces look generic (style, layout, code) and putting an adversarial critic on the end.

```
brief -> ui-reference-analyst -> ui-art-director -> ui-ux-architect -> ui-visual-designer -> ui-builder <-> ui-qa-critic -> ship
```

## Use it

In Claude Code, from this repo:

```
/ui-agents a modern skin for the Redline Modular synth, dark glass, fits a 1366x768 laptop. Reference: <attach screenshots>
```

Outputs land in `design/<slug>/`: brief, references, direction, UX, design system (`DESIGN.md` + `tokens.css`), the mock, QA rounds with screenshots.

## Use it in every project

Copy the agents and command once, and they work everywhere:

- Mac/Linux: `cp .claude/agents/ui-*.md ~/.claude/agents/ && cp .claude/commands/ui-agents.md ~/.claude/commands/`
- Windows (PowerShell): `Copy-Item .claude\agents\ui-*.md $env:USERPROFILE\.claude\agents\; Copy-Item .claude\commands\ui-agents.md $env:USERPROFILE\.claude\commands\`

Also copy `design/UI_AGENTS.md` and `design/tools/ui_audit.py` into each project's `design/` folder (the agents read them from there).

## Files

| File | Role |
|---|---|
| `design/UI_AGENTS.md` | Standards: principles, scales, anti-slop list, Polish Gate, plugin addendum |
| `design/tools/ui_audit.py` | Objective checks: contrast, tiny text, off-scale spacing, overflow, console errors, squint images |
| `.claude/agents/ui-*.md` | The six agents |
| `.claude/commands/ui-agents.md` | The `/ui-agents` lead command |
| `design/UI_AGENTS_MASTER_PROMPT.md` | One paste-able prompt that recreates everything above in any project |
| `design/tools/build_master_prompt.py` | Regenerates the master prompt from the files above |

## Audit tool

```
python3 design/tools/ui_audit.py path/to/index.html --out design/audit
python3 design/tools/ui_audit.py contrast '#e7eefc' '#0a1020'
```

Needs `pip install playwright pillow` and `playwright install chromium`.
