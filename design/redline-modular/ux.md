# REDLINE MODULAR: UX spec for the full-screen, two-rack rework

Slug: `redline-modular`. Mode: rework of `ui/index.html`. The look is approved and stays: walnut frame, black brushed panels, cream lettering, red accents, skirted bakelite knobs (`design/knobs/SKIRTED_KNOB.md`), chrome jacks, patch cables. This file changes structure, size and navigation only.

Assumptions (stated, not asked):
1. Mick's hard requirement overrides the brief: full screen, edge to edge, two stacked racks, keyboard underneath, no fit-to-one-screen shrinking.
2. "Visible without panning" for Basic Voice means every module that patch uses, with every cable end on screen at once.
3. The 8 px numeral exception is not needed. Numerals render at 10 px or more, so the audit can reach zero text-size errors with no exception.
4. Module order on screen may differ from the TOP/BOT array order. The arrays, IDs and `window.Redline.modules` stay as they are. Only a new render-order list is added (section 2.5).

---

## 1. Why the current layout fails (numbers from the source and `current-fit.png`)

The cabinet is a fixed 1692 x about 1008 px poster. TOP has 14 modules, BOT has 13, each 116 px wide, plus 2 x 34 px cheeks. Fit logic scales the whole poster down to fit one window.

| Measure | 1366x768 | 1920x1080 |
|---|---|---|
| Viewport after the 46 px toolbar and padding | 1330 x 676 | 1884 x 988 |
| Scale chosen (height limits it) | **0.67** | **0.98** |
| Poster drawn at | 1134 x 676 | 1658 x 988 |
| Side space wasted (letterbox) | 232 px (17%) | 262 px (14%) |
| Module title, authored 9.5 px | 6.4 px | 9.3 px |
| Module sub, 7 px | 4.7 px | 6.9 px |
| Knob label, 7.4 px | 5.0 px | 7.3 px |
| Value read-out, 7 px | 4.7 px | 6.9 px |
| Jack label, 6.8 px | 4.6 px | 6.7 px |
| Switch caption, 6.6 px | 4.4 px | 6.5 px |
| Scale numerals (5.6 units on a 54 px knob = 4.9 px) | 3.3 px | 4.8 px |
| Knob (54 px authored) | 36 px | 53 px |
| Jack (24 px authored) | 16 px, under the 24 px target | 23.5 px |

Causes, in order of weight:
1. **Fit shrinks text with the layout.** Text was authored at 4.4 to 9.5 px (even at 100% it fails the 10 px floor), then multiplied by 0.67. Nothing on the module panels reaches 10 px at any zoom the app picks.
2. **Too many modules for one screen.** To get a 6.8 px jack label up to 10 px the poster must be drawn at 1.47x, which is 2487 px wide. That is 182% of a 1366 px window. No tuning of the fit fixes this; the rack must scroll, so it cannot be one poster.
3. **Height is the binding limit, not width.** The poster is 1.68:1 and the laptop window is 1.78:1, so 17% of the width is unused while the height is the problem.
4. **Cables hide labels.** At 0.67 scale, the 8 Basic Voice cables run across jack labels (the screenshot shows GATE IN, OUT, PITCH IN struck through).
5. **Keyboard starved.** The keybed gets about 170 of 1008 px, then shrinks with everything else.

Ground rule for the rework: **text is never scaled below its authored size. Only space is managed.** Panels get compact, wide module layouts at a true 10 px floor; what does not fit across pans.

---

## 2. New layout

### 2.1 Structure

```
+----------------------------------------------------------------------------------+
| TOOLBAR  brand  preset  SAVE INIT CLEAR CABLES PANIC  MIDI  [FULL SCREEN] readout |  40
+--+-------------------------------------------------------------------------------+--+ walnut rail 8
|W | TOP RACK pane: modules pan left/right (voice group at the left)               |W |  264
|  | [chip]  cable edge indicators sit on this pane's left and right edges [chip]  |  |
+--+ [TOP RACK] [<] [ minimap: LPF ENV1 ENV2 ... |window| ... REVERB ] [>]       +--+  24 walnut rail
|W | BOTTOM RACK pane                                                              |W |  264
|  |   KBD   OSC 1   MIXER A   VCA 1   MAIN OUT  | OSC 2 ...                       |  |
+--+ [BOTTOM RACK] [<] [ minimap ........................................ ] [>]    +--+  24 walnut rail
|W | KEYBOARD: [pitch][mod] REDLINE plate | upper manual / lower manual            |W |  136
+--+-------------------------------------------------------------------------------+--+ walnut rail 8
```
W = walnut cheek, 10 x s px wide. The page itself never scrolls at 1366x768 or larger. The panes are the only scrolling areas, and they have no scrollbars.

Hero: the left edge of both racks, where the Basic Voice signal path sits (keyboard, oscillator, mixer, filter, envelope, VCA, output). The playing surface as a whole is the focus; the voice group anchors it.

### 2.2 Scale rule (one number, called `s`)

All sizes inside the panes and keyboard are authored at `s = 1` and multiplied by `s` (CSS custom property `--s`). Chrome (toolbar, walnut rails, minimap, edge chips) is fixed px.

```
fixedChrome = toolbar 40 + rails 8 + 24 + 24 + 8 = 104
s_h  = (windowHeight - 104) / 664          (664 = 2 x 264 + 136)
s_w  = windowWidth / 1168                  (1168 = widest voice group 1148 + cheeks 20)
s    = clamp( min(s_h, s_w, 1.5), 1, 1.5 )
```
- At exactly 1366x768, s_h = 1.000 and s = 1.000.
- If width limits `s` (s_w below s_h), spare height goes to the keyboard (taller keys). Nothing letterboxes.
- If s_h is above 1.5, spare height also goes to the keyboard. No rack grows beyond 1.5x; the racks show more modules instead.
- Minimum window: 1024 x 744. Between 744 and 767 high, s stays 1 and the keyboard shrinks from 136 to 112 (manuals 46 px each). Below 744 high, the whole page scrolls vertically and the toolbar stays pinned. The 10 px floor wins over fitting everything.
- Browser zoom or Windows DPI scaling changes the CSS-pixel window and so changes `s`. That is expected.
- Cheek width, screws, cable stroke, plug size, knob and jack art all use `s`.

### 2.3 Exact sizes

| Item | 1366 x 768 (s = 1.000) | 1920 x 1080 (s = 1.470) |
|---|---|---|
| Toolbar | 40 | 40 |
| Walnut rails (top, between racks x2, bottom) | 8 / 24 / 24 / 8 | 8 / 24 / 24 / 8 |
| TOP rack pane height | 264 | 388 |
| BOTTOM rack pane height | 264 | 388 |
| Keyboard height | 136 | 200 |
| Sum | 40+8+264+24+264+24+136+8 = **768** | 40+8+388+24+388+24+200+8 = **1080** |
| Cheek (each side) | 10 | 14.7 |
| Pane width | 1346 | 1890.6 |
| Module width, 4 columns | 268 | 394 |
| Module width, 3 columns | 204 | 300 |
| Module width, 2 columns | 140 | 206 |
| Column pitch | 64 | 94 |
| Knob art (skirted) | 48 | 70.6 |
| Jack art / jack cell | 24 / 64 x 38 | 35.3 / 94 x 56 |
| Wheel (pitch, mod) | 34 x 92 | 50 x 135 |
| Manual height (2 manuals + 6 gap) | 56 each | 82 each |
| White key width (36 per manual, capped at 26 x s) | 26 (manual 936 wide) | 38.2 (manual 1376 wide) |

Module width = columns x 64 + 12 (6 px padding each side). Columns follow the module's widest row (knobs, switches or jacks). 4-column: LOW PASS, VC LFO, ENVELOPE 1 to 4, DELAY, PHASER, REVERB, KEYBOARD, OSCILLATOR 1 to 4, MIXER A and B, ATTENUATORS A and B, MULTIPLES. 3-column: HIGH PASS, BAND PASS, SAMPLE & HOLD, VCA 1 and 2. 2-column: NOISE, MAIN OUTPUT. Knob columns and jack columns share one grid, so a knob sits above the jacks around it.

Module anatomy at s = 1 (264 high), top to bottom:

| Zone | Height | Content |
|---|---|---|
| Padding | 6 | screws at corners as now |
| Head | 32 | title 11 px, sub 10 px, rule, status LED where it exists |
| Knob row | 74 | knob 48, label 12, value line 12 (stepped knobs only, space always reserved), gaps 2 |
| Gap | 8 | |
| Switch row | 30 | only on KEYBOARD, VC LFO, REVERB, OSCILLATORS; hit area 24 high minimum |
| Slack | 0 to 106 | blank brushed panel; jacks are anchored to the bottom |
| Jack well | 46 (1 row) or 90 (2 rows) | row = 38 (jack 24 + label 12 + gap 2); well padding 4, row gap 6 |
| Padding | 6 | |

Worst case is OSCILLATOR and KEYBOARD: 6 + 32 + 74 + 8 + 30 + 8 + 90 + 6 = 254 of 264 (10 px spare). ATTENUATORS (8 jacks, 4 knobs) is lighter: 6 + 32 + 74 + 90 + 6 = 208.

### 2.4 Type scale (px, rendered at s = 1; multiply by s inside panes and keyboard)

| Role | Size | Weight / tracking | Notes |
|---|---|---|---|
| Module title | 11 | 800, +0.06em | was 9.5, rendered 6.4 |
| Module sub | 10 | 600, +0.12em | was 7 |
| Knob label | 10 | 700, +0.04em | was 7.4 |
| Value read-out (stepped knobs) | 10 | 800, +0.04em, `#ffd77a` | was 7 |
| Jack label | 10 | 800, +0.03em | output labels stay dark on the cream pill |
| Switch caption | 10 | 700, +0.04em | was 6.6 |
| Scale numerals "0" and "10" | 10 | 700 | SVG `.kt` font-size 13 units in the 62-unit viewBox on a 48 px knob = 10.06 px; placed at radius 30 units so they clear the skirt (radius 19.5) |
| Note text (MULTIPLES) | 10 | 700, +0.14em | was 7.5 |
| Wheel label (PITCH, MOD) | 10 | 800, +0.12em | was 7.5 |
| Keyboard plate: REDLINE | 15 | 800, +0.5em | unchanged |
| Keyboard plate: sub line | 10 | 700, +0.3em | was 8 |
| Toolbar buttons, rack buttons, minimap, edge chips | 10 | 700, +0.08em | fixed px, not scaled |
| Toolbar read-out | 11 | 700, +0.1em | fixed |
| MIDI label in toolbar | 10 | 700 | was 9 |

Distinct sizes: 10, 11, 15 (plus SVG numerals at 10.06). Font stack unchanged. Contrast: sub `#a29e90` on the dark panel measures above 5:1; chip text is cream on near-black.

Fit check for the longest labels at 10 px in the 64 px column: "FEEDBACK", "BANDWIDTH", "MOD WHEEL", "VELOCITY", "TRIANGLE", "BEND RANGE". The builder measures them in the audit browser (Arial or Liberation, wider than Arial Narrow). If any exceeds 62 px, first drop its tracking to 0, then, only as a last resort, shorten that one label (section 7).

### 2.5 Render order (module order on screen)

TOP and BOT arrays, IDs and `window.Redline.modules` stay exactly as they are. Add two id lists used only by the renderer, so DOM order, tab order and visual order all match:

```
VIEW_TOP = lpf env1 env2 env3 env4 | hpf bpf | lfo1 lfo2 noise | delay phaser reverb
VIEW_BOT = kbd osc1 mixA vca1 main | osc2 osc3 osc4 | mixB vca2 | att1 att2 mult
```
`|` is an 8 px (x s) walnut divider between groups. The first group in each list is the voice group: the Basic Voice patch lives there. TOP groups: shape (low pass plus envelopes), other filters, modulation, effects. BOT groups: played voice, extra oscillators, second mixer and VCA, attenuators and multiples.

Widths at s = 1: TOP 3164 + 24 dividers = 3188. BOT 3228 + 24 = 3252. Maximum pan: TOP 3188 - 1346 = 1842. BOT 3252 - 1346 = 1906.

### 2.6 What is visible without panning, default Basic Voice patch, racks at pan 0

**1366 x 768 (s = 1, pane 1346 wide)**

| Rack | Fully visible | Partly visible |
|---|---|---|
| TOP | LOW PASS (0 to 268), ENVELOPE 1, 2, 3 (to 1072), ENVELOPE 4 (to 1340; 6 px to spare) | none |
| BOTTOM | KEYBOARD (0 to 268), OSCILLATOR 1, MIXER A, VCA 1 (to 1008), MAIN OUTPUT (to 1148) | OSCILLATOR 2: 190 of 268 px, after the 8 px divider |

Every module and jack Basic Voice uses is fully on screen. Its 8 cables (kbd.pitch to osc1.cv, kbd.gate to env1.gate, osc1.out to mixA.in1, mixA.out to lpf.in, lpf.out to vca1.in1, env1.out to vca1.cv1, vca1.out to main.inl, vca1.out to main.inr) have both ends visible, so **zero edge chips** appear. The keyboard, wheels and both minimaps are visible at the same time. Nothing needs scrolling to play it.

**1920 x 1080 (s = 1.47, pane 1890.6 wide)**

| Rack | Fully visible | Partly visible |
|---|---|---|
| TOP | LOW PASS, ENVELOPE 1, 2, 3 (to 1576) | ENVELOPE 4: 314 of 394 px |
| BOTTOM | KEYBOARD, OSCILLATOR 1, MIXER A, VCA 1, MAIN OUTPUT (to 1688) | OSCILLATOR 2: 191 of 394 px |

Both racks start at pan 0 on first open. The check for both sizes: every `from` and `to` jack of the 8 cables is inside its pane's visible rectangle.

### 2.7 Keyboard section (always visible, never scrolls)

- Height 136 x s at the default (more when spare height is given to it, section 2.2). It sits under both racks, between the cheeks.
- Left: the brushed plate with the pitch wheel and mod wheel side by side (34 x 92 each, gap 14, labels at 10 px), then the REDLINE brand lockup. Wheels stay with the keyboard.
- Right: two stacked manuals (upper, lower), 61 keys each as now (MIDI 36 to 96). Manuals flex to fill the space left of the plate, capped at 36 white keys x 26 x s. Key positions are percentages of the manual width so they never drift.
- Key heights at s = 1: white 53, black 33. Black key width 0.62 of a white key.
- No cable is ever drawn over the keyboard. The cable layer is clipped above it.

### 2.8 Pane framing

- Each pane is a scroll container: `overflow-x: scroll`, scrollbar hidden, `overflow-y: hidden`, `overscroll-behavior: contain`, `touch-action: pan-x`, scroll padding 16 x s. Native scrolling gives touch momentum and focus scrolling for free.
- Edge fade: when more modules exist beyond an edge, a 24 x s px black gradient (35% to 0) sits on that edge. It does not take pointer events. It tells the eye there is more.
- Walnut rails and module shadows keep today's look. The pane gets the existing inset shadow so modules read as seated in a rack.

---

## 3. Cables across off-screen modules

The cable layer becomes one SVG that covers the window (`position: fixed`, pointer-events none except plugs and hit paths). It uses real screen coordinates from each jack's `getBoundingClientRect()`, so no zoom maths is needed. It is redrawn on pan (requestAnimationFrame, at most once per frame), on resize, on any cable change, and when `s` changes. Clip regions:

- Visible stroke clip: both pane rectangles plus the rail between them. Cables never paint over the toolbar or the keyboard.
- Hit and plug clip: the pane rectangles only. The minimap strips and buttons in the rails always stay clickable.

An end is **on screen** when its jack centre is inside its pane's visible rectangle, inset 12 x s px from each edge. Otherwise it is **off screen**.

| Case | What is drawn |
|---|---|
| Both ends on screen | Full cable with two plugs (as today). |
| One end on screen, the other off | The cable runs from the visible plug to the pane edge where it leaves the clip, the stroke fades over its last 24 px, and an **edge chip** sits at that exit point. No plug on the off-screen side. |
| Both ends off screen, the path crosses visible area | The visible stretch is drawn with no plugs, with a chip where it enters and a chip where it leaves. |
| Both ends off screen, nothing crosses | Nothing drawn. The minimap blocks of both modules show a cable dot (below). |

**Edge chip.** 24 px tall (fixed), width fits the text, flush with the pane's left or right edge, black `#0b0907` fill, 3 px bar in the cable's colour on the inner side, a chevron pointing off screen, then the other module's short name in 10 px cream, for example `‹ OSC 1`. Chips sit at the cable's exit y, sorted, and nudged apart to at least 26 px. At most 5 chips per edge; beyond that a `+N` chip stands in for the rest.

**Click-to-reveal.**
1. Click a chip. The pane that holds the other end pans (250 ms, ease-out) to centre that module, clamped to the pan limits.
2. When it arrives, that jack gets a pulsing ring in the cable's colour for 2 x 250 ms, and the cable brightens until the pointer moves or Esc is pressed.
3. The read-out shows `Cable to OSCILLATOR 1 · WAVE OUT (bottom rack)`.
4. If the other end is in the other rack, only that rack pans.
5. After the pan the first end may now be off screen. A chip appears at that end's pane edge, so the same gesture goes back.
- `+N` chip: each click reveals the next hidden cable and the chip shows how many are left.
- Hover a chip: the cable brightens, the read-out names both ends, the tooltip reads `Show OSCILLATOR 1 · WAVE OUT`.
- Chips are focusable. They sit in the DOM directly after their rack's controls, before that rack's minimap. `Enter` or `Space` reveals.

**Minimap cable dots.** Every minimap block shows up to 3 small dots (6 px, cable colours) for cables that end in that module. Dots appear only for jacks currently off screen. This is how you read the signal flow when the plugs are out of sight.

**Dragging a cable (new or picked up).**
- Auto-pan: when the pointer is within 48 x s px of a pane's left or right edge (or beyond it, even outside the window), that pane pans toward that edge. Speed = (0.15 + 0.85 x depth) x 1200 x s px/s, where depth is 0 at the zone's inner edge and 1 at the edge or beyond. The pane under the pointer's y position pans; over the minimap strip or keyboard nothing pans.
- While panning, the hit test (`elementFromPoint`) re-runs every frame, so a jack sliding under a still pointer highlights correctly.
- Dwell-jump: hold a dragged cable over a minimap block for 350 ms and that rack pans to centre that module.
- Mouse wheel and Shift+wheel also pan the pane under the pointer during a drag.
- Drop on a valid jack: connect (unchanged). Drop elsewhere: the cable is discarded (unchanged). Esc: cancel the drag and restore a picked-up cable to where it was (new; the old code lost it).
- The dragged cable's loose end stays under the pointer in screen space, so it can cross from one rack to the other freely.

---

## 4. Navigation

### 4.1 Mouse

| Gesture | Result |
|---|---|
| Drag blank panel, rail or edge of a module | Pans that rack. Cursor `grab`, then `grabbing`. 4 px threshold before it starts so clicks stay clicks. Release keeps a short flick (up to 300 ms, ease-out). |
| Drag a knob | Unchanged value drag (vertical, Shift for fine). Never pans. |
| Drag a jack or plug | Unchanged cable drag. Never pans, but auto-pans at the edge (section 3). |
| Wheel on blank panel, title, rail | Vertical wheel pans that rack sideways (1 notch = 80 x s px). Horizontal wheel or trackpad two-finger pans directly. |
| Shift + wheel anywhere on a rack | Pans, even over a knob. |
| Wheel on a knob | Unchanged knob step, **unless** that rack was panned by wheel in the last 250 ms. Then the wheel keeps panning, so a fast scroll never changes a value by accident. |
| Wheel on keyboard, toolbar | Nothing. The page never scrolls. |
| Double-click a knob | Unchanged reset to default. |
| Click a minimap block | That rack pans to centre the module (250 ms). |
| Drag the minimap window box | Pans that rack live. |
| Click `<` or `>` at the minimap ends | Pan 80% of the pane width, clamped. |
| Click "TOP RACK" or "BOTTOM RACK" | Pan to the start (the voice group). |
| Right-click or double-click a cable | Unchanged: removes it. |

### 4.2 Keyboard

| Key | Result |
|---|---|
| Tab order | toolbar buttons, TOP rack knobs and switches in visual order, TOP chips, TOP minimap (one stop, roving), BOTTOM rack knobs and switches, BOTTOM chips, BOTTOM minimap, pitch wheel, mod wheel. |
| Tab to a control that is off screen | The pane scrolls it into view with 16 x s px of margin (250 ms). Only that pane moves. |
| Left / Right on the minimap | Move between blocks (roving focus). Enter or Space centres the module. |
| Home / End on the minimap | Pan to the start / end of that rack. |
| `[` and `]` | Pan the rack that last had focus or the pointer, one page left / right. (Not used by the computer-key piano: `a w s e d f t g y h u j k o l`.) |
| Arrow keys on a knob or switch | Unchanged. |
| Esc | Cancel a cable drag, clear the reveal highlight. |
| Focus ring | Existing 2 px `#f0cf7a`, extended to chips, minimap blocks and rack buttons. |

### 4.3 Touch

- Swipe left or right on any blank part of a rack, or on a knob (knobs use `touch-action: pan-x`, so a sideways swipe pans and a vertical drag turns the knob), scrolls natively with momentum.
- Jacks and plugs use `touch-action: none`: a touch on them always starts a cable.
- Cable drag near an edge auto-pans exactly as with a mouse.
- Tap a minimap block or chip: same as click. Blocks are at least 24 px high (fixed) and 52 px wide at 1366; a coarse pointer adds 4 px of hit padding around chips.
- Known gap: jack cell height 38 is under the 44 px touch guideline; knob (48) and chips pass. Windows plugin use is mouse first; see Suggestions.

### 4.4 Minimap (the "section jump")

Sits in each rack's walnut rail, 24 high, fixed size. Left to right: rack button (72 wide, "TOP RACK" or "BOTTOM RACK"), `<` (24 x 24), the strip, `>` (24 x 24). Strip blocks are proportional to module widths with a 2 px gap and 6 px group gaps. Each block holds the module's short name in 10 px cream on a black panel colour. The window box (1.5 px `#f0cf7a` outline, 12% yellow tint) shows what the rack currently shows. At 1366 the strip is about 1190 wide, so the smallest blocks (NOISE, OUT) are about 52 px and the widest (ENV, DELAY, PHASER, REVERB) about 100 px. All are at least 24 x 24.

Short names (minimap and chips only; the panel titles do not change):

| TOP | | BOTTOM | |
|---|---|---|---|
| lpf | LPF | kbd | KBD |
| env1-4 | ENV 1 to ENV 4 | osc1-4 | OSC 1 to OSC 4 |
| hpf | HPF | mixA, mixB | MIX A, MIX B |
| bpf | BPF | vca1, vca2 | VCA 1, VCA 2 |
| lfo1 | LFO | main | OUT |
| lfo2 | S&H | att1, att2 | ATT A, ATT B |
| noise | NOISE | mult | MULT |
| delay | DELAY | | |
| phaser | PHASER | | |
| reverb | REVERB | | |

### 4.5 Pan memory

Pan offsets are saved in design pixels (divided by `s`) under a new key `redline.view.v1` as `{top, bottom}`. The existing key `redline.state.v2` keeps its format. Its `zoom` field stays in `getState()` for compatibility and reports the current `s`; on load it is ignored. First open with no saved view: both racks at pan 0.

---

## 5. States

Columns cover: default, hover, focus-visible, active, disabled, selected, plus loading, empty, error, success under "Other". "n/a" means it does not exist for that element, and the reason is in the cell or the note below the table.

| Element | Default | Hover | Focus-visible | Active | Disabled | Selected / on | Other |
|---|---|---|---|---|---|---|---|
| Knob | skirted knob, white line shows value | read-out in toolbar shows value | 2 px yellow ring | drag; read-out live | n/a (no disabled params) | n/a | automation turns the pointer, no ring |
| Switch | lever at position, active caption yellow | read-out | ring | flips | n/a | caption yellow | |
| Jack (empty) | chrome hex nut | glow `#ffcf6a`, read-out | n/a (not keyboard reachable today) | cable drag from it | n/a | n/a | `bad`: red glow 350 ms on invalid drop (unchanged); `hot`: glow while a dragged cable is over it |
| Cable | coloured, shadow, shine | brighter | n/a | n/a | n/a | reveal highlight (brighter, 2 pulses at the jack) | off-screen end: fades into a chip |
| Plug | chrome with colour centre | grab cursor | n/a | drag picks up the cable | n/a | n/a | |
| Edge chip | black, colour bar, name | cable brightens, tooltip | ring | pressed 1 px down | n/a | n/a | `+N` variant; absent when no cable end is off screen |
| Pane | walnut-framed rack | n/a | n/a | grabbing cursor while panning | n/a | n/a | empty n/a (modules are fixed); edge fade where more content lies |
| Minimap block | black, cream name | lighter fill, tooltip `Show OSCILLATOR 1` | ring | pressed | n/a | inside the window box = tinted | activity tick, cable dots |
| Minimap window box | yellow outline | grab cursor | n/a | dragging | n/a | n/a | |
| `<` `>` buttons | toolbar button style | brighter | ring | pressed | 40% opacity at the first / last position | n/a | |
| Rack button | toolbar style | brighter | ring | pressed | n/a | n/a | |
| Toolbar buttons | as today | as today | ring (new) | as today | n/a | n/a | |
| FULL SCREEN button | text FULL SCREEN | brighter | ring | pressed | hidden when the host cannot do fullscreen | label EXIT FULL SCREEN while on | error: read-out message (section 7) |
| MIDI LED | dark red | n/a | n/a | n/a | n/a | lit 90 ms on a note | |
| Key (white / black) | as today | n/a | n/a | pressed, `.on` look | n/a | held | |
| Pitch / mod wheel | as today | n/a | ring | drag | n/a | n/a | pitch springs to centre on release |
| Env LEDs, VU | as today | n/a | n/a | n/a | n/a | lit | |

Whole-screen states: first run shows both racks at pan 0, Basic Voice loaded, no chips. With no audio running the VU LEDs are dark and layout is unchanged. "Nothing selected" does not exist in this UI because there is no selection concept. Loading and error do not apply to controls; the only error is a refused fullscreen request.

---

## 6. Fit and zoom controls become

| Old | New |
|---|---|
| `-` button, zoom value, `+` button | **Removed.** Size is automatic from the window (section 2.2). |
| `FIT` button | **Removed.** Nothing to fit; the layout always fills the window. |
| `100%` button | **Removed.** |
| Ctrl + wheel zoom | **Removed.** The page does not capture it. |
| New: `FULL SCREEN` button | Toolbar, after MIDI, before the read-out. Toggles the browser Fullscreen API on the document. Label becomes `EXIT FULL SCREEN` while on. Esc leaves (browser behaviour). Hidden if `document.fullscreenEnabled` is false (some plugin web views); the window itself still fills edge to edge. |
| Window resize | Recomputes `s`, module sizes, cable positions, minimap and chips in one frame. Pan positions are kept in design px. |

In the native plugin the host window is the viewport. The same layout applies. Default window 1366 x 768 or the largest the host allows. Aspect is free, not locked, because the layout is not a poster.

---

## 7. Copy changes

Everything else (module titles, subs, knob and jack labels, preset names, toolbar buttons SAVE, INIT, CLEAR CABLES, PANIC, their tooltips) stays word for word.

| Where | Text |
|---|---|
| Fullscreen button | `FULL SCREEN` / `EXIT FULL SCREEN` |
| Fullscreen button tooltip | `Fill the whole screen. Press Esc to leave.` / `Leave full screen` |
| Fullscreen refused | Read-out: `Full screen was blocked. Use F11 or the window controls instead.` |
| Rack buttons | `TOP RACK`, `BOTTOM RACK`; tooltip `Go to the start of the top rack` / `Go to the start of the bottom rack` |
| `<` / `>` | aria-label and tooltip `Pan left` / `Pan right` |
| Minimap block | text = short name (4.4); tooltip and aria-label `Show OSCILLATOR 1`; `aria-current` on blocks inside the window |
| Chip | visible `OSC 1` with a chevron; aria-label `Cable to OSCILLATOR 1, WAVE OUT, bottom rack. Show`; overflow `+2` aria-label `2 more cables off screen. Show next` |
| Read-out on chip hover or click | `Cable to OSCILLATOR 1 · WAVE OUT (bottom rack)` |
| Read-out, cable drag cancelled with Esc | `Cable cancelled` |
| Brass plate on the mid rail | Removed (REDLINE already appears in the toolbar and on the keyboard plate; the 9 px text could not meet the floor in a 24 px rail that now holds the minimap) |
| MIDI label | unchanged text, 10 px |
| Contingency only if a label overflows its 62 px column at 10 px in the audit font | `MOD WHEEL` becomes `MOD WHL`. Nothing else may be shortened. Record any change in `decisions.md`. |

New read-outs and tooltips are sentence case; labels on the hardware stay in capitals like the rest of the panel.

---

## 8. Host behaviour (native plugin and web view)

| Event | What the person sees |
|---|---|
| Host automation moves a knob on screen | The knob pointer turns and the value line updates. No focus ring. If that control is the one being dragged, the drag wins until release (unchanged). |
| Host automation moves a knob in an off-screen module | The racks do not move. That module's minimap block brightens for 250 ms (activity tick). |
| Bypass | Bypass is a host control. The UI does not change layout; cables and knobs stay visible. No bypass control is added. |
| Preset change | Params and cables load as today. The racks keep their pan. Chips appear or disappear for cable ends off screen. Read-out `Preset: <name>` as today. |
| Window resize | Layout recomputes at once. No letterbox, no pan jump (pan kept in design px). |
| First open, no audio | Basic Voice loaded or last saved patch, both racks at pan 0, VU dark. |
| Session reload | Params and cables restored (`redline.state.v2`), pans restored (`redline.view.v1`), preset name selected. |
| Second instance | Each native instance keeps its own state in the host session. In a plain browser both read the same keys, so the last one saved wins (existing behaviour). |

Real-time marking (values that cross from the audio thread to the UI):

| Element | Live data | Path |
|---|---|---|
| VU LEDs on MAIN OUTPUT | output level | `Redline.setVU(level)` from an atomic read on a 30 Hz UI timer |
| Env LEDs and KEYBOARD LED | gate and note state | `setLED` (note events, not audio samples) |
| MIDI LED | note activity | `midiActivity()` |
| Minimap activity tick | parameter change notification | same path as automation, no audio data |
| Everything else (knob positions, cables, pan, chips) | none | UI thread only |

---

## 9. Interaction detail for native widgets (unchanged unless stated)

- Knob drag: vertical, 170 px per full range (stepped: 28 px per step), Shift = 4x finer, divided by `s` so feel is the same at any size. Wheel: 0.02 per notch (Shift 0.005; stepped: one step). Double-click: default. Arrow keys: 0.05 (Shift 0.01). Value read-out in the toolbar while hovering or dragging; stepped values also show in the value line under the knob.
- Typing an exact value, right-click menu, MIDI-learn: do not exist today and are not added.
- Wheels (pitch, mod): vertical drag, 80 x s px per full range, pitch springs to centre.
- Keys: pointer down plays, release stops; computer keys unchanged.
- Cable drag, plug pick-up, one cable per input, colours: unchanged.

---

## 10. Edge cases

| Case | Behaviour |
|---|---|
| Long labels | None should exceed the column at 10 px except possibly `MOD WHEEL` (section 7 contingency). Titles never wrap; the widest is `SAMPLE & HOLD` in a 204 px module. |
| 20 or more cables | Chips cap at 5 per edge plus `+N`; minimap dots cap at 3 per block. |
| Cable between two modules in the same off-screen stretch | Not drawn; both blocks show a dot. |
| Window narrower than the voice group | The rack just pans and chips appear. Nothing breaks. |
| Window wider than 1920 | `s` caps at 1.5; racks show more modules; the keyboard takes spare height. |
| Host moves a control while it is being dragged | Drag wins (unchanged). |
| Picked-up cable, Esc | Cable returns to its original input (new). |
| Reduced motion | No 250 ms smooth pan, no flick inertia, no pulse ring: jumps are instant and the reveal highlight is a static glow for 750 ms. |
| Browser blocks Fullscreen | Button hidden or read-out message (section 7). |
| Window shorter than 744 | Page scrolls vertically, toolbar pinned, text stays at 10 px. |

---

## 11. Verification checklist

Run on the built `ui/index.html`, at device scale factor 1.

1. **Audit, zero text-size errors:** `python3 design/tools/ui_audit.py ui/index.html --allow-scroll --viewports 1366x768@1,1920x1080@1`. Expect no `text renders at ... (floor 10px)` lines at either viewport and no page-overflow ERROR (INFO is fine). Contrast WARNs on image backgrounds are checked by eye.
2. **Exact sizes (Playwright):** at 1366x768, `s` = 1.000; pane heights 264 and 264; keyboard 136; sum 768; `documentElement.scrollWidth == clientWidth` and `scrollHeight == clientHeight`. At 1920x1080, `s` = 1.47 (+/- 0.01); panes 388; keyboard 200.
3. **Computed sizes at 1366x768:** `.mtitle` 11, `.msub` 10, `.klabel` 10, `.kval` 10, `.jlabel` 10, `.sw span` 10, SVG `.kt` rendered at 10 or more (use `getScreenCTM`), `.wcell span` 10, `.note` 10, toolbar `.btn` 10. No text element under 10.
4. **Targets:** knobs 48 x 48 or more, jack cell 64 x 38 or more, switch 24 high or more, minimap blocks and chips 24 x 24 or more. The audit shows no "under 24px" WARN for interactive elements.
5. **Basic Voice without panning:** at 1366x768 and 1920x1080 load `Basic Voice` and assert lpf, env1, kbd, osc1, mixA, vca1, main are entirely inside their pane's rectangle; every cable end is on screen; edge chip count is 0; keyboard and both minimaps are inside the viewport.
6. **Cables off screen:** load `Lead — Classic`. Assert chips appear for cables whose end is off screen, click one, assert the right pane's scroll changes, the target jack gets the pulse, and the return chip appears. Repeat with `Pad — Phaser & Reverb` (phaser and reverb are in the TOP rack's last group).
7. **Auto-pan:** start a cable at `kbd.pitch`, move to the bottom pane's right 20 px, hold 1 s; pane `scrollLeft` increases; drop on `osc3.cv`; the cable connects. Esc during a drag restores a picked-up cable.
8. **Wheel safety:** wheel over a knob right after a wheel-pan within 250 ms does not change the value; after 300 ms it does.
9. **Navigation:** minimap click centres a module; dragging the window box pans; `<` `>`, `[` `]`, Home, End work; Tab into an off-screen knob scrolls only that pane; touch emulation: swipe on a knob pans, vertical drag turns it.
10. **Fullscreen:** the button toggles `document.fullscreenElement`, the label changes, Esc leaves, and the layout recomputes with no letterbox.
11. **API and data unchanged:** `Object.keys(window.Redline)` identical to before; `modules.top` (14) and `modules.bottom` (13) arrays identical and in original order; `params().length` = 98; `jacks().length` = 121; `getState()` still has `params, cables, zoom, preset`; `redline.state.v2` readable by the old version; DOM `data-p` and `data-j` ids unchanged.
12. **Resize:** 1366x768, 1280x768, 1024x744, 1440x900, 1920x1080, 2560x1440: no horizontal page scroll. Below 744 high, vertical page scroll with the toolbar pinned.
13. **Look kept:** side-by-side crops of a knob, jack, switch, module panel, cheek and wheel against `current-fit.png` show the same art.
14. **Screenshots to deliver:** 1366x768 and 1920x1080 default; each with one rack panned to its end; one with edge chips visible (Lead preset); one mid-cable-drag at an edge; a DPR 2 crop of an oscillator module. Zero console errors.

---

## 12. Parameter map

No parameter, ID, range or default changes. UI ids are `<module>.<param>`; the native port uses lowerCamelCase (`osc1.freq` becomes `osc1Freq`). The UI stores every value as normalised 0 to 1. Real units and skew are decided by the DSP and listed so the builder writes one `plugin.json`. All rows are host-automatable. No row is added or removed. (There is no `plugin.json` for this plugin in the repo yet.)

Types: **float** = continuous 0..1. **choice** = stepped with the listed names, value = index (UI stores index / (steps - 1)). **bool** = switch.

| Module (id) | Param id | Display name | Type | Min..Max | Default | Unit | Skew |
|---|---|---|---|---|---|---|---|
| lpf | cutoff | CUTOFF | float | 0..1 | 0.70 | Hz (DSP) | log |
| lpf | emph | EMPHASIS | float | 0..1 | 0.25 | resonance | linear |
| lpf | amt | CV AMT | float | 0..1 | 0.50 | cv depth | linear |
| lpf | slope | SLOPE | choice 12, 24 | index 0..1 | 24 (index 1) | dB/oct | n/a |
| hpf | cutoff / emph / amt | CUTOFF, EMPHASIS, CV AMT | float | 0..1 | 0.30 / 0.10 / 0.50 | Hz / res / cv | log / lin / lin |
| bpf | center / bw / amt | CENTER, BANDWIDTH, CV AMT | float | 0..1 | 0.50 / 0.40 / 0.50 | Hz / octaves / cv | log / lin / lin |
| lfo1 | freq / pw / fm | FREQUENCY, PULSE W, FM AMT | float | 0..1 | 0.35 / 0.50 / 0 | Hz / % / cv | log / lin / lin |
| lfo1 | range | RANGE | choice SLOW, MED, FAST | index 0..2 | MED (1) | | n/a |
| lfo1 | trk | FREE / KBD TRK | bool | 0..1 | 0 (FREE) | | n/a |
| lfo2 | rate / slew / lvl | RATE, SLEW, LEVEL | float | 0..1 | 0.40 / 0.20 / 0.80 | Hz / ms / gain | log / lin / lin |
| noise | color / level | COLOUR, LEVEL | float | 0..1 | 0.50 / 0.70 | tilt / gain | lin |
| env1, env2, env3, env4 | attack / decay / sustain / release | ATTACK, DECAY, SUSTAIN, RELEASE | float | 0..1 | 0.05 / 0.40 / 0.60 / 0.35 | s / s / level / s | log, log, lin, log |
| delay | time / fb / tone / mix | TIME, FEEDBACK, TONE, MIX | float | 0..1 | 0.40 / 0.35 / 0.50 / 0.30 | s / % / tone / % | log, lin, lin, lin |
| phaser | rate / depth / fb / mix | RATE, DEPTH, FEEDBACK, MIX | float | 0..1 | 0.30 / 0.50 / 0.40 / 0.40 | Hz / % / % / % | log, lin, lin, lin |
| reverb | size / decay / damp / mix | SIZE, DECAY, DAMPING, MIX | float | 0..1 | 0.50 / 0.50 / 0.40 / 0.30 | size / s / damp / % | lin, log, lin, lin |
| reverb | freeze | NORMAL / FREEZE | bool | 0..1 | 0 (NORMAL) | | n/a |
| kbd | glide / tune | GLIDE, FINE TUNE | float | 0..1 | 0 / 0.50 | ms / cents | lin |
| kbd | oct | OCTAVE | choice -2, -1, 0, +1, +2 | index 0..4 | 0 (index 2) | octaves | n/a |
| kbd | bend | BEND RANGE | choice 1, 2, 5, 12 | index 0..3 | 2 (index 1) | semitones | n/a |
| kbd | legato | RETRIG / LEGATO | bool | 0..1 | 0 (RETRIG) | | n/a |
| osc1, osc2, osc3, osc4 | freq | FREQUENCY | float | 0..1 | 0.50 | Hz (DSP) | log |
| osc1..4 | range | RANGE | choice LO, 32', 16', 8', 4', 2' | index 0..5 | 8' (index 3) | feet | n/a |
| osc1..4 | wave | WAVEFORM | choice TRI, SHK, SAW, RMP, SQR, PLS | index 0..5 | SAW (index 2) | | n/a |
| osc1..4 | pw | PULSE W | float | 0..1 | 0.50 | % | lin |
| osc1..4 | kbd | FREE / KBD | bool | 0..1 | 1 (KBD) | | n/a |
| mixA, mixB | l1 / l2 / l3 / l4 | IN 1, IN 2, IN 3, IN 4 | float | 0..1 | 0.60 / 0.60 / 0.40 / 0 | gain | lin |
| vca1, vca2 | gain / amt / bias | GAIN, CV AMT, BIAS | float | 0..1 | 0.70 / 0.80 / 0 | gain / cv / level | lin |
| att1, att2 | a1 / a2 / a3 / a4 | 1, 2, 3, 4 | float | 0..1 | 0.50 each | gain | lin |
| main | vol / pan | VOLUME, BALANCE | float | 0..1 | 0.70 / 0.50 | gain / pan | lin |

Totals: 49 parameters in the top array plus 49 in the bottom array = 98; 121 jacks (43 top, 78 bottom). The skew column is advice to the DSP builder only; the UI knob stays linear in its 0..1 value.

**UI-only state (not host parameters, not automatable):** rack pan `top` and `bottom` in design px (`redline.view.v1`); full screen on or off (not saved); reveal highlight (not saved). None has a parameter ID.

---

## Suggestions (not in scope, not designed)

1. Keyboard patching: jacks are not reachable by Tab today, so cables can only be made with a pointer. A keyboard-only cable flow would finish the accessibility story.
2. A "patch view" that fades everything except the cables and the modules they touch, to read signal flow at a glance.
3. A touch mode (`pointer: coarse`) that grows jack cells to 44 px high with a 1.15 scale floor.
4. A skin switch once the layout is stable (never replacing this look).
