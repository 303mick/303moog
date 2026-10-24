# UX: The Butcher (VST3 effect, 1100 x 680, direction C1 "Cold Counter")

Date: 2026-10-24. Author: ux-architect. Inputs: `design/UI_AGENTS.md`, `brief.md`, `references.md`, `direction.md`.

## Assumptions (stated, not asked)
1. The whole plugin is ONE front page. Nothing is a separate page or tab. Occasional things sit behind disclosure: the preset browser (a drawer), the Save popover, right-click menus, per-target Brutality amounts (a ring handle) and typed values.
2. No feature is added. Two ideas from `references.md` are deliberately left out because the brief does not list them: Randomise/Mutate buttons and per-module bypass. The only bypass is the host's own, which the UI reflects. Two small read-outs exist only to show the state of listed controls: the filter curve (Type, Cutoff, Resonance) and the gate glyph (Gate shape).
3. Grind's four modes are named Soft, Hard, Fold, Rectify (the brief says "4 types" only). DSP may rename them; the copy deck is the one place to change.
4. Brutality reaches each target through an **amount** per target (references.md open question 5). Amounts belong to the existing macro, not a new module.
5. With the host transport stopped, Cleave free-runs at the last known tempo (DSP to confirm); with no known tempo it uses 120 BPM and the footer says "not synced".
6. Text-size risk: the minimum window is 0.88x (968 x 598), so 11 px legends render at 9.7 px there (see section 7). The old 880 x 544 minimum (0.8x) was rejected because it rendered them at 8.8 px. Set legends at 11 px and values at 12 px, never lower.
7. Base unit 4 px; spacing from the dense scale (2, 4, 6, 8, 12, 16, 24). Margin 12, gutter 8, plate padding 8. Derived exceptions are marked "derived".

---

## 1. Job to be done

**Who and when.** A producer in Ableton Live on Windows, mid-session, loop already playing, headphones or monitors loud, a 1366x768 laptop, short on time. They want to wreck one sound fast, hear it change at once, and keep the result. They do not read manuals.

**Success in one sentence.** Within ten seconds of opening the plugin the person has muted a few slices, pushed Brutality up, heard the sound turn nasty, and saved it as a preset.

**Five-second test.** A first-time viewer must understand: (1) the black card is my audio, cut into 16 slices I can switch off; (2) the green plate is one big dial, Brutality, that makes everything harsher; (3) the four panels below are Cleave, Grind, Filter and Output, and Mix is in Output.

---

## 2. Information architecture

### 2.1 View map
One main view. Overlays, all closed with Esc, none changes the window size:

```
Front page (always visible)
 |- Preset browser drawer  (from Browse or the preset tag; covers the right column only)
 |- Save popover           (from Save; hangs under the Save button)
 |- Context menu           (right-click a control: enter value, reset, MIDI learn, link)
 |- Value entry            (inline, replaces one readout)
 |- Tooltip                (hover 600 ms; the footer mirrors it at once)
```

The drawer covers the Brutality plate, Filter and Output (x 648 to 1088) and leaves the hero, Cleave and Grind live, so the person sees and hears a preset land on the cut chart. Its header carries a small Brutality read-out because the plate is hidden.

### 2.2 Ranked controls and displays

**Tier 1 (one hero, two actions)**
| Rank | Item | Why |
|---|---|---|
| Hero | Slice display: live waveform, cut-chart grid, 16 clickable step zones | It is the product: see where the cuts fall, mute steps on the grid itself |
| Action 1 | Brutality dial | "Dial how brutal it is" is the primary job |
| Action 2 | Save | "Save the result as a preset" ends the primary job |

**Tier 2 (visible, normal weight):** preset name with previous/next, Browse, Mix, Cleave Chance, Cleave Grid, Grind Mode, Drive, Crush, Filter Type, Cutoff, and the four Brutality target chips (link switches with amounts).

**Tier 3 (visible, quiet):** Swing, Gate shape, Reverse chance, Pitch drop, Triplet, Dotted, gate glyph, Rate, Tone, Resonance, filter curve, Output gain, input and output meters, favourite stamp, A/B slots, Init, step numbers, ruler, slice count, footer.

**Behind disclosure:** preset search, tag filters and the list, Save name and tags, per-target Brutality amounts (ring handle), MIDI learn, typed values, reset (double-click or menu), copy A to B.

Cut or capped: meter scale has three marks only; the ruler is 10 px at most; modules have no power buttons.

---

## 3. Layout

### 3.1 Grid
- Window 1100 x 680, aspect 1.6176:1, locked. Margin 12, gutter 8, plate padding 8.
- Two blocks share edges. Left block x 12 to 640 (628 wide), right block x 648 to 1088 (440 wide). Filter's left edge and the Brutality plate's left edge are both x 648; Grind's right edge and the hero's right edge are both x 640.
- Rows: header 0 to 48; hero row y 60 to 400 (340); module row y 408 to 640 (232); footer y 652 to 680.
- Step grid: 16 columns, pitch 37, first column x 30, width 592 (derived: (628 - 16 x 37) / 2 = 18 inset). Group lines after steps 4, 8, 12 are heavier lines, not extra space, so the pitch is constant.
- Knob cell 72 tall: knob 40, name 11 px caps, value 12 px tabular, 4 spacing. Brutality dial and Mix are the only larger knobs.

### 3.2 Exact rectangles at 1100 x 680 (x, y, w, h)
| Region | x | y | w | h | Notes |
|---|---|---|---|---|---|
| Header / preset bar | 0 | 0 | 1100 | 48 | Steel, 1 px bottom edge |
| Wordmark | 12 | 8 | 156 | 32 | |
| Previous preset | 176 | 12 | 24 | 24 | |
| Preset tag | 204 | 8 | 292 | 32 | Kraft tag, slab name, punched hole |
| Next preset | 500 | 12 | 24 | 24 | |
| Favourite stamp | 532 | 12 | 32 | 24 | |
| A/B slots | 572 | 8 | 68 | 32 | Two 32 x 32 tag slots, 4 gap |
| Browse | 648 | 10 | 144 | 28 | |
| Save | 800 | 10 | 144 | 28 | Tier 1 action |
| Init | 952 | 10 | 136 | 28 | |
| **Hero display (price card)** | **12** | **60** | **628** | **340** | Notched top corners |
| Hero info strip | 12 | 60 | 628 | 28 | Slice count left, grid text right |
| Hero step-number row | 12 | 88 | 628 | 24 | 16 tags 24 x 16; leader lines drop from them |
| Hero zones and waveform | 30 | 112 | 592 | 260 | 16 columns of 37 x 260 |
| Hero ruler and hint strip | 12 | 372 | 628 | 28 | Bar.beat ticks, first-run hint |
| **Brutality plate** | **648** | **60** | **440** | **340** | Enamel green, 14 radius, one chipped corner |
| Plate title and subtitle | 664 | 68 | 408 | 40 | |
| Dial (scale and face) | 760 | 116 | 216 | 216 | Centre (868, 224), scale radius 108, 270 degree sweep |
| Target chips (4) | 664, 768, 872, 976 | 348 | 96 each | 44 | Gap 8; link switch with amount |
| **Cleave plate** | **12** | **408** | **340** | **232** | |
| **Grind plate** | **360** | **408** | **280** | **232** | |
| **Filter plate** | **648** | **408** | **220** | **232** | |
| **Output plate** | **876** | **408** | **212** | **232** | Right edge 1088 |
| Footer / status line | 0 | 652 | 1100 | 28 | Status left at x 12; tempo and audio status right |
| Preset drawer (open) | 648 | 56 | 440 | 592 | Elevation 3, over the right column |
| Save popover (open) | 800 | 42 | 288 | 168 | Under Save, elevation 2 |

Module internals (inside 8 padding):
| Module | Header | Selector row | Controls |
|---|---|---|---|
| Cleave | y 416, h 24: chip + subtitle | y 448, h 24: "Grid" label, five 40-wide segments (1/4 1/8 1/16 1/32 1/64), Trip and Dot toggles 40 wide | Row 1 y 484 to 556: Chance, Swing, Gate shape (cells 108 wide at x 20, 128, 236). Row 2 y 560 to 632: Reverse chance, Pitch drop, gate glyph |
| Grind | y 416 | y 448: four 66-wide mode segments (x 368 to 632) | 2 x 2: Drive (x 368), Crush (x 500) at y 484 to 556; Rate, Tone at y 560 to 632 |
| Filter | y 416 | y 448: three 68-wide type segments (x 656 to 860) | Curve well x 656, y 480, 204 x 64 (light inset on the plate, not the black card); Cutoff and Resonance, 102 wide, y 556 to 628 |
| Output | y 416 | none | Left column x 884, w 116: Mix (56 dia) y 452 to 540, Output gain (40 dia) y 548 to 620. Meters card x 1008, y 448, 72 x 180 (black) |

### 3.3 Wireframe (1 character about 11 x 22 px; schematic, the table above is authoritative). Muted steps 6 and 11 shown as X.

```
+--------------------------------------------------------------------------------------------------+
| THE BUTCHER   < [ Low End Rack            ] > (*) [A|B]  [ Browse ] [ Save ] [ Init ]            |
+--------------------------------------------------------------------------------------------------+
|+-------------------------------------------------------+ +--------------------------------------+|
|| 12 of 16 slices on                  Grid 1/16, straight| | BRUTALITY                            ||
||   1  2  3  4 | 5  6  7  8 | 9 10 11 12 |13 14 15 16   | | Pushes Chance, Drive, Crush, Cutoff  ||
||  :    :     |       :    |    :       | :        :    | |             .-''''''''-.             ||
||:|::|::|:   |:|:\ /:|::|:|   :|:\ /:|:|:|:   :|::|:|    | |          .'              '.          ||
||:|::|::|:   |:|:\ /:|::|:|   :|:\ /:|:|:|:   :|::|:|    | |        |      \   6.2       |        ||
||-|--|--|--.-|-|- X -|--|-|-.--|- X -|-|-|--.--|--|-|    | |         '.                .'         ||
||-|--|--|--.-|-|- X -|--|-|-.--|- X -|-|-|--.--|--|-|    | |             '-.______.-'             ||
||:|::|::|:   |:|:/ \:|::|:|   :|:/ \:|:|:|:   :|::|:|    | |                                      ||
||:|::|::|:   |:|:/ \:|::|:|   :|:/ \:|:|:|:   :|::|:|    | |  0    2    4    6    8    10        ||
||  :    :     |       :    |    :       | :        :    | | [Chance]  [Drive ]  [Crush ]  [Cutoff]||
||  1.1         |2.1         |3.1         |4.1            | | [+60%  ]  [+70%  ]  [+70%  ]  [-40%  ]||
|| Click a slice to mute it. Drag Brutality up.           | |                                      ||
|+-------------------------------------------------------+ +--------------------------------------+|
|+-----------------------------+ +-----------------------+ +------------------+ +-----------------+|
|| CLEAVE  slice and stutter   | | GRIND distort, crush  | | FILTER           | | OUTPUT          ||
|| Grid [1/16] [Trip] [Dot]    | | [Soft|Hard|Fold|Rect] | | [LP][BP][HP]     | |         In Out  ||
||   (o)       (o)       (o)   | |  (o)       (o)        | | ____      /\     | |  (O)      || || ||
|| Chance    Swing    Gate     | | Drive      Crush      | | ----\____/  \____| | Mix       || || ||
|| 100%      0%      Hard      | | 0 dB       24 bit     | |  (o)       (o)   | | 100%      || || ||
||   (o)       (o)      /^\    | |  (o)       (o)        | | Cutoff    Reso   | |           || || ||
|| Reverse   Pitch    (shape)  | | Rate       Tone       | | 1.2 kHz   35%    | |  (o)      || || ||
|| 0%        0 st              | | Full       +0         | |                  | | Gain 0.0dB|| || ||
||                             | |                       | |                  | |           -6 -3 ||
|+-----------------------------+ +-----------------------+ +------------------+ +-----------------+|
| Hover a control to see what it does.             No audio at input     120.0 BPM, synced to host |
+--------------------------------------------------------------------------------------------------+
```
HERO = the black card at top left (x 12, y 60, 628 x 340). In the wireframe labels are shortened to fit; real labels are in section 6.

### 3.4 Grouping logic
- **Hero and Brutality share one row** with the same top and bottom edge (y 60, y 400): they are done together (see the cuts, push the macro).
- **One plate per module**, one lane-colour header chip each. Mix lives in Output with the meters: level and blend are one decision.
- **Brutality's four targets appear twice, on purpose:** chips under the dial (summary and link switch) and rings on the knobs (the reach). The chips sit on the plate, so the macro talks about other modules in one place only.
- **Presets are in the header only.** The kraft tag is the only warm object on screen, so "keep" reads as a different job from "play".

### 3.5 Window size
| Item | Value |
|---|---|
| Default | 1100 x 680 (708 with host frame, fits 728 usable on 1366x768) |
| Minimum / maximum | 968 x 598 (0.88x) / 1650 x 1020 (1.5x). Why the minimum is not the brief's 880 x 544: at 0.8x the 11 px legends would render at 8.8 px, below the 10 px floor; at 0.88x they render at 9.7 px. See decisions.md |
| Resize | Uniform scale of the whole canvas, aspect locked 1100:680, no reflow, no scrollbars. Size remembered per instance and as the default for new ones |
| Pixel density | Vector; hairlines stay 1 device-independent px at 100 to 200% |
| Overlays | Scale with the canvas; never leave the window |

---

## 4. States inventory

"n/a" always carries a reason. Visual treatment belongs to the visual designer; this fixes what must exist.

### 4.1 Controls
| Control | Default | Hover | Focus-visible | Active | Disabled | Selected | Loading | Empty | Error | Success |
|---|---|---|---|---|---|---|---|---|---|---|
| Knob (all continuous parameters) | Value arc from start (centre if bipolar), name and value always visible | Pointer `ns-resize`; footer shows name, value, tip; tooltip at 600 ms | Two-tone ring around the knob | Value bubble above knob; footer shows range | n/a: no parameter is ever unavailable | n/a: no selection model | Arc greyed, value "..." until preset loaded (max 400 ms) | n/a | n/a: values clamp; arc end flashes once | n/a |
| Brutality-driven knob (Chance, Drive, Crush, Cutoff) | Knob plus thin lane-colour ring | Ring and its chip brighten together | Focus ring outside the modulation ring | Bubble reads "Base 40%, now 62%"; ring moves with knob | n/a | n/a | As knob | n/a | n/a | n/a |
| Segmented selector (Grid, Mode, Type) | One segment filled | Segment lightens | Ring on group; Left and Right change selection | Segment sinks 1 px | n/a | Filled segment | As knob | n/a | n/a | n/a |
| Trip and Dot toggles | Off, flat | Lightens | Ring | Pressed | n/a: always allowed on every grid | On: filled in lane colour | n/a | n/a | n/a | n/a |
| Step (on) | Zone lines, waveform visible, leader line to number | Zone lightens, number tag lifts, footer "Step 7 is on. Click to mute." | Dashed 2 px ring on zone, number underlined | Edge flashes 80 ms; drag paints | n/a | n/a: on/off is its value | Baseline only | n/a | n/a | n/a |
| Step (muted) | Zone at 35% plus ink-stamp X; number struck. Never dimming alone | As on | As on | As on | n/a | n/a | n/a | n/a | n/a | n/a |
| Step (playing) | Zone lit Cleave amber 120 ms, decays 250 ms | n/a: transient | n/a | n/a | n/a | n/a | n/a | n/a | n/a | n/a |
| Brutality dial | Needle at value, ticks 0 to 10, value large in centre | Face lifts 1 px; footer "Brutality 6.2. Drives four controls." | Ring around dial | Needle follows; bubble; rings update live | n/a | n/a | Needle at 0, value "..." | n/a | n/a | n/a |
| Target chip | Linked: swatch filled, amount shown | Matching ring pulses once | Ring | Pressed toggles link | n/a | Linked is the selected look; unlinked is hollow swatch with "Off" | n/a | n/a | n/a | Footer "Chance is linked to Brutality." |
| Preset tag | Name in slab type | Underline; tooltip with full name | Ring | Opens browser | n/a | n/a | "Loading..." in place of name | "Init" when nothing loaded | Name struck by an ink X, footer error | "Saved" stamp for 1.5 s |
| Previous, Next | Flat chip | Lightens | Ring | Pressed | Disabled when the list has one item or none | n/a | n/a | n/a | n/a | n/a |
| Favourite stamp | Hollow ring | Ring darkens | Ring | Pressed | Disabled on Init: nothing to keep | Filled ink stamp, rotated 6 degrees | n/a | n/a | n/a | n/a |
| A/B slots | Active slot lit | Inactive lightens | Ring; Left and Right switch | Pressed | n/a | Active slot | n/a | Inactive slot reads "empty" until first switch (then copies the active one) | n/a | n/a |
| Browse | Button | Lightens | Ring | Pressed, opens drawer | n/a | Pressed look while drawer open | n/a | n/a | n/a | n/a |
| Save | Tier 1 button | Lightens | Ring | Pressed, opens popover | n/a | Pressed look while popover open | "Saving..." up to 1 s | n/a | Inline error in popover | "Saved" stamp on tag |
| Init | Secondary button | Lightens | Ring | Pressed, resets everything | n/a | n/a | n/a | n/a | n/a | Footer "Reset to Init." |
| Filter curve | Drawn from Type, Cutoff, Resonance | n/a: not interactive | n/a: not focusable | Redraws live | n/a | n/a | Flat line | n/a | n/a | n/a |
| Meters | Bars, peak hold 1.5 s | Tooltip with peak dB | n/a: not focusable | Click clears hold | n/a | n/a | Bars empty | Empty bars with no audio | Clip: stamped "CLIP" mark until clicked | n/a |
| Browser row | Name, tags | Row highlights | Ring | Pressed loads | n/a | Loaded preset: kraft-tint row with stamp | Skeleton rows while scanning | See 4.3 | "Can't load" suffix | n/a |
| Search field | Placeholder "Search presets" | Border darkens | Ring | Typing filters (120 ms debounce) | n/a | n/a | n/a | n/a | n/a | n/a |

### 4.2 Views and global states
| State | What the person sees | Behaviour |
|---|---|---|
| **First run** | Preset Init, all 16 steps on, Brutality 0; hint in the ruler strip "Click a slice to mute it. Drag Brutality up." | Hint disappears after the first step click or dial drag; flag stored in plugin settings, not the preset |
| **Nothing selected** (Init or no preset loaded) | Tag reads "Init"; favourite stamp disabled; no highlighted row in the browser | Editing and Save work as normal |
| **Loading / no-audio at open** | Window appears at once; arcs greyed, values "...", hero shows a chalk line and "Loading preset" | Input accepted after load (up to 400 ms). Over 2 s: footer adds "Still loading. Large preset." No spinner |
| **No audio** (no signal for 2 s) | Grid and heavy zone lines stay; waveform is a flat chalk baseline; centred "No audio. Play something through this track."; meters empty; footer right "No audio at input" | Everything stays editable. Text fades out in 150 ms when signal arrives |
| **Bypassed** (host bypass) | Round ink stamp "BYPASSED" across the hero, tilted 6 degrees; waveform at 60%; no slice flashes; Brutality plate and modules at 70%; footer "Bypassed by the host. Sound passes through untouched." | Controls stay editable so a sound can be prepared; heard when bypass ends |
| **Muted steps** | Ink-stamp X, zone at 35%, number struck; hero strip count updates ("9 of 16 slices on") | Muting one step never changes another. A muted step passes dry signal if Mix is under 100%, else silence (DSP to confirm) |
| **Parameter automated by the host** | The control moves by itself; hollow 6 x 6 diamond beside its name while values arrive, fading 1 s after the last change | Grabbing the control sends begin-edit; host values to it are ignored until mouse-up; then the readout glides to the host value in 80 ms |
| **Brutality automated or MIDI-driven** | Dial moves, linked rings follow, diamond beside the plate title | Same grab rule |
| **Brutality rings** | See 4.4 | |
| **Browser open** | Kraft drawer slides in 150 ms ease-out over the right column; hero, Cleave, Grind live | Esc, click outside or Close dismisses in 150 ms ease-in. Loading a preset keeps it open so many can be auditioned |
| **Save popover open** | Paper popover under Save: name, tags, Save, Cancel | Enter saves; Esc cancels |
| **Error** | Tag struck by an ink X; footer message with a red-orange stamp mark (state colour); no modal | Messages in section 6.7 |
| **Success** | "Saved" stamp over tag, footer line | Gone after 1.5 s |
| **Minimum size** | Whole canvas at 0.88x (968 x 598) | Section 7 |

### 4.3 Browser states
| State | Content |
|---|---|
| Default | Empty search, tag "All" selected, A to Z, loaded preset highlighted and scrolled into view |
| Loading | Six skeleton rows; count "Scanning presets..." |
| Empty (none installed) | "No presets found. The factory presets did not install. Reinstall The Butcher, or choose Init to start from scratch." |
| Empty search | "No presets match "sub". Clear the search or pick a different tag." |
| Empty favourites | "No favourites yet. Click the stamp beside a preset to keep it here." |
| Error | "Couldn't read the presets folder. Check that it exists and that Windows lets this plugin open it, then press Rescan." (Rescan button only here) |
| Large | Virtualised; scrollbar 8 px wide always visible, thumb 24 px minimum; count "37 of 12,000" |

### 4.4 Brutality ring states (modulation-linked)
Ring: 2 px arc, 4 px outside the knob's value arc, in the target's lane colour. A faint track shows the 270 degree range; the bright arc runs from the base to the heard value; a 4 px dot marks the heard value.

| State | Trigger | Appearance |
|---|---|---|
| Linked, idle | Brutality 0 | Track and a dot at the base; no bright arc |
| Linked, active | Brutality above 0 | Bright arc base to heard value, dot at the end; readout shows the heard value |
| Linked, at limit | Heard value reaches range end | Arc ends in a short tick, dot flattens |
| Base being dragged | Person drags the knob | Ring moves with the knob; bubble shows base and heard values |
| Amount being set | Person drags the ring dot | Ring thickens to 3 px; chip amount updates live |
| Chip hover | Pointer on chip | Matching ring brightens and pulses once (120 ms) |
| Brutality automated | Host or MIDI | Rings follow at display rate, no pulse |
| Unlinked | Chip off or menu | Ring removed; chip hollow with "Off" |
| Bypassed | Host bypass | Rings 60%, static |
| Disabled | n/a: unlink is the off state | |

---

## 5. Interaction spec

### 5.1 Mouse
| Target | Gesture | Result and feedback |
|---|---|---|
| Knob | Drag vertical | 160 px covers the full range; bubble at mouse-down; displayed value eased 10 to 20 ms, DSP smoothed separately |
| Knob | Shift-drag | Tenth of normal speed |
| Knob | Wheel | 1% per notch, Shift 0.1%; on a segmented selector one notch = one segment. The wheel never scrolls the window |
| Knob | Double-click | Resets to default, 150 ms glide, footer "Chance reset to 100%" |
| Knob | Click readout | Value entry in place; Enter confirms, Esc cancels |
| Knob | Right-click | Menu: Enter value, Reset, Learn MIDI CC (Forget MIDI CC 21 when mapped); for the four targets also Unlink from Brutality or Link to Brutality, and Set Brutality amount |
| Knob | Hover 600 ms | Tooltip; footer mirrors at once |
| Segmented selector | Click | Selects, 80 ms fill |
| Step | Click | Toggles; X stamp in or out over 80 ms; footer reports |
| Step | Click-drag across steps | Paints every touched step to the opposite of the first step's state |
| Step number | Click | Same as its step |
| Brutality dial | Drag vertical | 200 px = 0 to 10; Shift tenth speed; wheel 0.1 per notch; double-click returns to 0.0 |
| Target chip | Click | Toggles link; ring in or out in 150 ms |
| Ring dot | Drag along ring | Sets amount -100% to +100%, snaps to 0 within 3%; double-click on the ring restores the default |
| Preset tag | Click | Opens browser |
| Previous, Next | Click | Loads previous or next in the current list (the filtered list while the browser is open); wraps |
| Favourite stamp | Click | Toggles; footer "Added to favourites" or "Removed from favourites" |
| A, B | Click inactive | Swaps the whole parameter state; footer "Switched to B". Right-click: Copy A to B, Copy B to A |
| Save | Click | Opens popover (5.2) |
| Init | Click | Resets every parameter, step and amount to defaults; links return to linked |
| Meters | Click | Clears peak hold and CLIP |
| Browser row | Click / double-click | Loads / loads and closes the drawer |
| Row stamp | Click | Toggles that preset's favourite |

### 5.2 Save popover
Name field focused with text selected, pre-filled with the current name ("Untitled" from Init). Tag chips (Slice, Stutter, Heavy, Subtle, Drums, Bass, Vocals, Full mix), multi-select. Button reads Save. Factory name: field note "Factory presets can't be overwritten." and button reads Save as new. Existing user name: line "A preset called "X" already exists." and button reads Replace. Esc or Cancel closes, no change.

### 5.3 Keyboard (works when the plugin window has focus; every key action has a mouse equivalent)
| Key | Where | Result |
|---|---|---|
| Tab, Shift+Tab | Anywhere | Order: Previous, tag, Next, favourite, A, B, Browse, Save, Init, steps 1 to 16, Brutality dial, four chips, Cleave, Grind, Filter, Output |
| Up, Right / Down, Left | Knob | +1% / -1% (dial +0.1 / -0.1); Shift = tenth |
| Page Up, Page Down | Knob | +10% / -10% |
| Home, End | Knob | Minimum, maximum |
| Delete, Backspace | Knob | Reset to default |
| Enter | Knob | Value entry |
| Left, Right, Home, End | Step | Previous, next, step 1, step 16 |
| Space | Step, chip, toggle, button | Activates |
| Left, Right | Segmented selector, A/B | Changes selection |
| Esc | Overlay open | Closes topmost; else drops focus |
| Ctrl+F | Browser open | Focuses search |
| Up, Down, Enter, Page Up, Page Down | Browser list | Move, load, page of 16 rows |

### 5.4 Touch and pen
Targets at least 24 x 24 at default (smallest: Trip, Dot at 40 x 24). Touch drag = mouse drag; long press 500 ms = right-click. No pinch.

### 5.5 MIDI learn and automation
- Learn: dashed ring on the control, footer "Listening. Move a knob or fader on your controller. Esc cancels." First CC maps it: "Chance is now on CC 21." After 10 s: "No controller moved. Nothing was mapped."
- Learnable: every knob, the dial and Mix. Not selectors or steps. Mappings live in plugin state, not presets.
- Host-exposed parameters: every knob, selector, toggle, the 16 steps and Brutality. Brutality amounts and link states are saved in presets and are not host parameters.
- Mouse-down sends begin-edit, mouse-up end-edit.

### 5.6 Brutality: how it drives four controls
**Rule.** Heard value = the target's own knob value (base) + Brutality (0 to 10 as 0 to 1) x amount, in the target's own range. At Brutality 0 nothing moves.

| Target | Default amount | At Brutality 10 |
|---|---|---|
| Chance | +60% | Rises by 60% of its range, to a maximum of 100% |
| Drive | +70% | Rises by 70% of its range |
| Crush | +70% | Bit depth falls by 70% of its range (harsher) |
| Cutoff | -40% | Falls by 40% of its range on the log scale (filter closes) |

**Visual.** Each driven knob wears the ring (4.4) and shows the heard value; the base appears while dragging. The chip repeats the amount and is the link switch, in the lane colour of its module (Cleave, Grind, Grind, Filter). No lines cross the screen.

**Override and unlink, four ways:**
1. Click the chip: unlinked, ring gone, chip "Off".
2. Right-click the knob, Unlink from Brutality.
3. Drag the knob: moves the base; the macro keeps adding its offset.
4. Drag the ring dot to 0%: still linked, no effect, ring collapses to a dot. Double-click the ring for the default.

### 5.7 Tooltips and read-outs
Tooltip at 600 ms, hides on mouse-down, never covers its control, stays inside the window. The footer repeats the tooltip at once, so no legend depends on a delay. Value read-outs are always visible; a bubble repeats them while dragging.

---

## 6. Copy deck

Sentence case everywhere except the module names and wordmark, which are all caps per the direction (stamped lettering). Buttons name outcomes; one name per action.

### 6.1 Header
| Element | Label | Tooltip |
|---|---|---|
| Wordmark | THE BUTCHER | none |
| Previous | (arrow glyph); accessible name "Previous preset" | Load the previous preset |
| Preset tag | Preset name; "Init" when none | Open the preset browser. Full name: {name} |
| Next | accessible name "Next preset" | Load the next preset |
| Favourite | accessible name "Favourite" | Keep this preset in your favourites |
| A, B | A, B | Compare two versions of this sound. Click to switch. Right-click to copy one to the other |
| Browse | Browse | Search and load presets |
| Save | Save | Save this sound as a preset |
| Init | Init | Reset every control to its starting value |

### 6.2 Hero
| Element | Text / tooltip |
|---|---|
| Strip left | "{n} of 16 slices on" |
| Strip right | "Grid {1/16}, straight" / "Grid {1/16}, triplet" / "Grid {1/16}, dotted" |
| Step tag | "Step {n}" / tooltip: "Step {n} is on. Click to mute it." or "Step {n} is muted. Click to switch it on." |
| Display tooltip | "Your incoming audio, cut into 16 slices. Click a slice to mute it. Drag across slices to paint." |
| Ruler | "1.1  2.1  3.1  4.1" (bar.beat of the first step in each group of four) |
| First-run hint | "Click a slice to mute it. Drag Brutality up." |
| No audio | "No audio. Play something through this track." |
| Loading | "Loading preset" |
| Bypassed stamp | BYPASSED |

### 6.3 Brutality
| Element | Text |
|---|---|
| Plate title | BRUTALITY |
| Subtitle | one control for Chance, Drive, Crush and Cutoff |
| Dial tooltip | Push Chance, Drive, Crush and Cutoff together. Drag up for more. Double-click to reset. |
| Scale | 0 2 4 6 8 10 (every second tick labelled) |
| Chip labels | Chance, Drive, Crush, Cutoff; amount "+60%" |
| Chip tooltip (linked) | Brutality moves Chance by {amount}. Click to unlink. |
| Chip tooltip (unlinked) | Chance ignores Brutality. Click to link. |
| Ring tooltip | Brutality amount for Chance: {amount}. Drag to change. Double-click to reset. |

### 6.4 Module controls (label, value format, tooltip)
| Control | Label | Value format | Tooltip |
|---|---|---|---|
| Cleave header | CLEAVE, subtitle "slice and stutter" | | Cuts the sound into slices and stutters them in time with your host tempo. |
| Grid | Grid | 1/4 1/8 1/16 1/32 1/64 | Length of each slice. |
| Triplet | Trip | on / off | Triplet timing: three slices in the space of two. |
| Dotted | Dot | on / off | Dotted timing: each slice is one and a half times as long. |
| Chance | Chance | 0 to 100% | How often an active slice actually plays. Lower for more gaps. |
| Swing | Swing | 0 to 100% | Pushes every second slice late for a shuffled feel. |
| Gate shape | Gate shape | Hard, 35% soft, Soft | How slice edges fade. Hard clicks, soft fades. |
| Reverse chance | Reverse chance | 0 to 100% | How often a slice plays backwards. |
| Pitch drop | Pitch drop | 0 to -24 st | Drops the pitch of each slice, like a tape slowing. |
| Gate glyph | (no label) | | Shape of one slice edge. |
| Grind header | GRIND, subtitle "distortion and crush" | | Distorts and crushes the sound. |
| Mode | Soft, Hard, Fold, Rectify | | Soft: rounded clipping. Hard: flat clipping. Fold: waveform folds back on itself. Rectify: flips the negative half up. |
| Drive | Drive | 0 to 36 dB | How hard the signal is pushed into the distortion. |
| Crush | Crush | 24 to 1 bit | Bit depth. Fewer bits, grittier sound. |
| Rate | Rate | Full, 1/2, 1/4 ... 1/64 | Sample-rate reduction. Lower rates give aliasing and a lo-fi edge. |
| Tone | Tone | -100 to +100 | Tilts the distorted sound darker or brighter. |
| Filter header | FILTER | | Shapes the sound after the distortion. |
| Type | Low-pass, Band-pass, High-pass | | Low-pass keeps lows, band-pass keeps a middle band, high-pass keeps highs. |
| Cutoff | Cutoff | 20 Hz to 20 kHz, shown as 1.2 kHz | Frequency where the filter starts to cut. |
| Resonance | Resonance | 0 to 100% | Boosts the sound at the cutoff for a sharper, ringing edge. |
| Filter curve | (no label) | | Shape of the filter. |
| Output header | OUTPUT | | Blend and level. |
| Mix | Mix | 0 to 100% | Blend between the original sound (0%) and the processed sound (100%). |
| Output gain | Output gain | -24 to +12 dB | Level after all processing. |
| Meters | In, Out | peak in dB | Input / output level. Click to clear the peak. |

### 6.5 Browser and Save
| Element | Text |
|---|---|
| Drawer title | Presets |
| Search placeholder | Search presets |
| Tag chips | All, Favourites, Slice, Stutter, Heavy, Subtle, Drums, Bass, Vocals, Full mix |
| Count | "{n} presets" / "{n} of {total} presets" |
| Drawer Brutality read-out | "Brutality {6.2}" |
| Close | Close |
| Rescan (error only) | Rescan |
| Row tooltip | "{name}. Click to load. Double-click to load and close." |
| Save popover title | Save preset |
| Name field label | Name |
| Tags label | Tags |
| Buttons | Save, Save as new, Replace, Cancel |
| Context menu items | Enter value, Reset to {default}, Learn MIDI CC, Forget MIDI CC {n}, Unlink from Brutality, Link to Brutality, Set Brutality amount, Copy A to B, Copy B to A |

### 6.6 Footer
| State | Text |
|---|---|
| Idle | Hover a control to see what it does. |
| Hover | The control's tooltip |
| Right, audio | "Audio in" / "No audio at input" |
| Right, tempo | "{120.0} BPM, synced to host" / "{120.0} BPM, not synced" |
| Confirmations | "Saved {name}." "Added to favourites." "Removed from favourites." "Switched to B." "Reset to Init." "{Control} reset to {value}." "{Control} is now on CC {n}." "Chance is linked to Brutality." "Chance is not linked to Brutality." |

### 6.7 Errors
| Case | Message |
|---|---|
| Preset fails to load | Couldn't load "{name}". The file is damaged or made by a newer version of The Butcher. Pick another preset, or choose Init. |
| Save fails | Couldn't save "{name}". The presets folder is read-only or full. Free some space or choose a different name, then try Save again. |
| Empty name | Give the preset a name to save it. |
| Name too long | Names can be 48 characters. Shorten it to save. |
| Presets folder unreadable | Couldn't read the presets folder. Check that it exists and that Windows lets this plugin open it, then press Rescan. |
| MIDI learn timeout | No controller moved. Nothing was mapped. |
| Clip | CLIP (stamp on meter). Footer: "Output went over 0 dB. Lower Output gain or Drive." |
| Init confirm (host without undo only) | Reset all controls to their starting values? Buttons: Reset, Cancel |

---

## 7. Edge cases
| Case | Behaviour |
|---|---|
| Long preset name | Tag is a fixed 292 px; text truncates with an end ellipsis at about 30 characters of slab type; tooltip and the browser show the full name. Names are limited to 48 characters in Save. Browser rows truncate in the middle so the end of the name stays readable |
| Very short name ("A") | Tag keeps its width; text left-aligned after the punched hole. Never resizes |
| 128 presets | Whole list fits about 8 screens; tag chips all enabled; scroll bar visible |
| 12,000 presets | Virtualised list, search debounced 120 ms, count always shown, tag chips with zero hits are disabled, favourites chip is the fast path. Scrollbar thumb never under 24 px. Opening scrolls to the loaded preset |
| No audio | Section 4.2. Steps still editable; flat baseline; text prompt |
| Transport stopped | Playhead stops at 1.1; slicing free-runs at last known tempo (assumption 5) |
| No host tempo | Footer "120.0 BPM, not synced" |
| Host automation moves a control while dragged | The person's value wins from mouse-down to mouse-up; incoming host values to that control are held; on release the readout glides to the host value in 80 ms; the diamond marker stays hollow while held. Host-written moves to other controls continue |
| Host automates a linked target's base | Ring follows the new base; no special state |
| Brutality moves while a target is dragged | Ring and heard value update; bubble shows both base and heard |
| Grid changed during playback | Step on/off values keep their step numbers; zones morph 150 ms; no audio click (DSP crossfades) |
| Triplet and Dotted together | Dotted wins; Trip switches off, footer "Dotted and triplet can't be combined. Dotted is on." |
| All 16 steps muted | Hero strip reads "0 of 16 slices on"; output is silent at Mix 100%; footer note once: "All slices are muted. Click a slice to switch it on." |
| Preset loaded with an unlinked target | Chip hollow, no ring; saved state respected |
| Window at 968 x 598 (minimum) | Whole canvas at 0.88x. Legends render at 9.7 px, the closest to the 10 px floor the layout allows; the old 880 x 544 would give 8.8 px. No information is carried by tertiary text alone (step numbers also have tag shapes; ruler is decorative). Dragging the host frame below 968 x 598 stops at the minimum. Decision recorded in decisions.md |
| Window at 1650 x 1020 | 1.5x; hairlines stay 1 device-independent px. Larger than a 1366x768 screen can show; the host clips it |
| 100 to 200% display scaling | Vector; no bitmaps |
| Mono input | Waveform shows the single channel; meters show one bar twice |
| Stereo input | Waveform shows the sum of left and right |
| Meter over 0 dB | CLIP stamp holds until clicked |
| Reduced motion | Slice flash becomes a 1-frame outline change, drawer opens without slide, ring pulses become a static brighten |

---

## 8. Handoff checks for the builder and critic
1. Rectangles in 3.2 are exact at 1100 x 680; every plate edge matches one of x 12, 352, 360, 640, 648, 868, 876, 1088 and y 60, 400, 408, 640.
2. Hero is the only element with a black price card above 200 px; the meters card is the only other.
3. Every control in the brief appears on the front page; browser internals, Save, menus and amounts are the only things behind disclosure.
4. Muted steps carry an X stamp, not dimming alone. Rings appear on exactly Chance, Drive, Crush, Cutoff.
5. No label is hover-only. Mix is visible at all times.
6. Window minimum is 968 x 598 and the maximum 1650 x 1020, aspect locked; the builder sets both as editor constraints.

---

## 9. Parameter map

Every control with a row here exists in the plugin; a control with no row does not. This table becomes `plugin.json`. Ranges and defaults come from sections 3, 5 and 6 (copy deck, wireframe, Reset tooltips). "(assumed)" marks a value ux.md did not state. Percent parameters are stored as 0 to 100 in the unit shown. Skew 1.0 means linear; 0.25 is the skew for Hz ranges (low frequencies get more of the knob travel). Choice and bool parameters are stepped.

### 9.1 Parameters
| id | Display name | Type | Min | Max | Default | Unit | Skew | Decimals | Automatable | Choices |
|---|---|---|---|---|---|---|---|---|---|---|
| cleaveGrid | Grid | choice | 0 | 4 | 2 (1/16) (assumed from wireframe) | | | 0 | yes | 1/4, 1/8, 1/16, 1/32, 1/64 |
| cleaveTriplet | Trip | bool | 0 | 1 | 0 (off) | | | 0 | yes | Off, On |
| cleaveDotted | Dot | bool | 0 | 1 | 0 (off) | | | 0 | yes | Off, On. Dotted wins when both are on (section 7) |
| cleaveChance | Chance | float | 0 | 100 | 100 | % | 1.0 | 0 | yes | |
| cleaveSwing | Swing | float | 0 | 100 | 0 | % | 1.0 | 0 | yes | |
| cleaveGate | Gate shape | float | 0 | 100 | 0 (Hard) | % soft | 1.0 | 0 | yes | Display: 0 reads "Hard", 100 reads "Soft", between reads "{n}% soft". Default 0 from the wireframe |
| cleaveReverse | Reverse chance | float | 0 | 100 | 0 | % | 1.0 | 0 | yes | |
| cleavePitchDrop | Pitch drop | float | -24 | 0 | 0 | st | 1.0 | 0 (assumed) | yes | |
| grindMode | Mode | choice | 0 | 3 | 0 (Soft) (assumed) | | | 0 | yes | Soft, Hard, Fold, Rectify |
| grindDrive | Drive | float | 0 | 36 | 0 | dB | 1.0 | 1 (assumed) | yes | |
| grindCrush | Crush | float | 1 | 24 | 24 | bit | 1.0 | 0 (assumed; stored continuous so Brutality can move it smoothly) | yes | |
| grindRate | Rate | choice | 0 | 6 | 0 (Full) | | | 0 | yes | Full, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64 (assumed: the copy deck gives "Full, 1/2, 1/4 ... 1/64", powers of two) |
| grindTone | Tone | float | -100 | 100 | 0 | | 1.0 | 0 | yes | Display with sign: "+0", "-35", "+60" |
| filterType | Type | choice | 0 | 2 | 0 (Low-pass) (assumed) | | | 0 | yes | Low-pass, Band-pass, High-pass |
| filterCutoff | Cutoff | float | 20 | 20000 | 20000 (assumed: filter open at Init) | Hz | 0.25 | 0 (shown as "1.2 kHz" above 1000 Hz, 2 decimals in kHz) | yes | |
| filterRes | Resonance | float | 0 | 100 | 0 (assumed) | % | 1.0 | 0 | yes | |
| mix | Mix | float | 0 | 100 | 100 | % | 1.0 | 0 | yes | |
| outputGain | Output gain | float | -24 | 12 | 0 | dB | 1.0 | 1 | yes | |
| brutality | Brutality | float | 0 | 10 | 0 | | 1.0 | 1 | yes | |
| step1 | Step 1 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step2 | Step 2 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step3 | Step 3 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step4 | Step 4 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step5 | Step 5 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step6 | Step 6 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step7 | Step 7 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step8 | Step 8 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step9 | Step 9 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step10 | Step 10 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step11 | Step 11 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step12 | Step 12 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step13 | Step 13 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step14 | Step 14 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step15 | Step 15 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |
| step16 | Step 16 | bool | 0 | 1 | 1 (on) | | | 0 | yes | Muted, On |

Notes for the builder:
- Parameter order for the host: grid and toggles, Cleave knobs, Grind, Filter, Output, Brutality, steps 1 to 16.
- **Brutality link amounts are not host parameters** (section 5.5). The four amounts (chance +60%, drive +70%, crush +70%, cutoff -40%; range -100 to +100, snap to 0 within 3%) and the four link on/off flags are saved in plugin state and in presets, so they have no row above. If the lead decides to make them automatable, add `brutalityChanceAmount`, `brutalityDriveAmount`, `brutalityCrushAmount`, `brutalityCutoffAmount` as float -100 to +100, defaults 60, 70, 70, -40, unit %, skew 1.0, 0 decimals.
- The four targets (cleaveChance, grindDrive, grindCrush, filterCutoff) hold the **base** value. The heard value (base plus Brutality x amount, section 5.6) is computed in the processor and shown on the ring; the host never sees it.
- Not parameters (state only, outside presets): MIDI CC mappings, window size, first-run hint flag. In presets: the parameters above plus amounts and link flags.
- Init resets every row to its default.

### 9.2 Live values crossing from the audio thread
These are read-only for the UI. The audio thread writes; the UI reads at display rate (about 30 to 60 Hz) through a lock-free structure (atomics or a single-writer ring buffer). The UI never blocks the audio thread.

| Value | Used by | Notes |
|---|---|---|
| Input level, peak, per channel | In meter, "No audio" state (no signal for 2 s), footer "Audio in" | Peak in dB; the UI keeps the 1.5 s peak hold and the CLIP latch |
| Output level, peak, per channel | Out meter, CLIP stamp | Same. Click on a meter clears hold and CLIP on the UI side only |
| Waveform data | Hero waveform | Downsampled min/max pairs of the summed input, one block per display frame, covering the 16-step window; mono and stereo handled on the audio side |
| Slice playhead | Playing-step flash, ruler, "playing" state | Current step index (0 to 15) and position in the step; also whether the slice was muted, chance-skipped or reversed, so the flash matches what is heard |
| Heard values of the four Brutality targets | Rings, readouts on Chance, Drive, Crush, Cutoff | Computed from base plus Brutality x amount; can be computed on the UI side from the parameters if the maths is shared |
| Host tempo and sync flag | Footer "{n} BPM, synced to host" or "not synced" | Updated when the host reports it |
| Host bypass flag | BYPASSED stamp, dimming | Written from the processor's bypass state |
