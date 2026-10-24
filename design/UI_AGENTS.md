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
