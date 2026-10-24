# Direction: The Butcher (VST3 effect, 1100 x 680)

Date: 2026-10-24. Author: art-director.

Assumptions (stated, not asked): the brief's "dark theme suits the audience" is an assumption, so I argue it below with evidence. No reference images exist; the palette hexes are my choices, not measured. Lane colours are a coding system (references.md section 4, Effectrix), not decoration. No existing look to protect; this is a new plugin, so there is no skin question.

---

## 1. Subject world

1. **Stainless prep counter under cold strip light**: tinted steel, a single hard top light, hairline specular edge.
2. **Black price card with chalk-white numerals**: the card a butcher slides into a display stand.
3. **Kraft butcher paper and luggage tag on string**: punched hole, notched corner, slab-serif number.
4. **Rubber stamp, imperfect single-colour ink**: round and rectangular marks, a state you can see at once.
5. **Cut-chart diagram**: numbered zones, hairline leader lines to labels (abstract waveform only, never a silhouette), plus the fixed-arc brass-and-enamel scale dial.

---

## 2. Light or dark, decided

The category default is near-black with one neon accent, and the research logs "legends that vanish on dark fascias" (Saturn 2) as a measured complaint. The subject is cold steel, which is light and mid-tone in real life, not black. Decision for the two candidates that go on show:

- **Mid-tone steel fascia** (not white; about 78% lightness) with **one dark inset: the hero display as a black price card**. Legends are ink on steel, 9:1 or better. The waveform sits on the only dark area in the window, so it wins the squint test by contrast alone, not by glow. Glare on a dark studio screen is limited because the large surface is mid-tone, not paper white.
- Temperature: **cool** for everything you play (steel, enamel), **warm** (kraft) only for what you keep (presets, tags). Temperature encodes the job.
- One candidate (C3) stays dark on purpose, to test the opposite, but with enamel green rather than black and white legends rather than grey.

---

## 3. Three candidates

### C1. Cold Counter
Concept: a stainless counter with the signal laid on a black price card, the cuts drawn as a butcher's cut chart, and the macro as a fused-enamel scale plate.

**Palette**
| Name | Hex | Role |
|---|---|---|
| Steel | `#C4CDD2` | window ground |
| Plate | `#D9E0E3` | module plates (lighter than ground, top light) |
| Ink | `#17212A` | all legends, values; 11:1 on Steel |
| Price card | `#12181D` | hero display well and meters only |
| Chalk | `#EEF2EC` | waveform trace, grid lines and numerals on the card |
| Enamel green | `#1D5C43` | the Brutality plate; the single accent |
| Kraft | `#B58B57` | preset tag and browser only (ink on it is 5.4:1) |

Lane coding (header chip fill, knob arc, and nothing else): Cleave `#B25A0C` (lit on card: `#F2A93B`), Grind `#7B2D5C`, Filter `#1F5F8B`, Output `#44535D`. Each is at least 3:1 on Steel. White text on each chip is at least 4.5:1.

**Type** (3 embedded files, all OFL)
- Barlow Condensed 600: module names, legends, caps labels. It descends from road and shop signage, so it reads as stamped lettering and stays legible at 11 px caps with +0.06em tracking. Chosen over Oswald/Bebas because those are the habit for "industrial".
- Barlow 500: numeric values and small text. Same drawing as the condensed face but with tabular figures (`font-variant-numeric: tabular-nums`; builder verifies), so values do not jitter while dragging.
- Zilla Slab 600: preset name and tag numbers only (slab tag-number vernacular, the one warm voice).

**Shape and material**
- Radius: step cells 1 px, controls and chips 2 px, module plates 4 px, the hero card 0 px with two notched (cut) top corners, the Brutality plate 14 px with a single chipped corner. Never equal across tiers.
- Edges: 1 px hairline top-left highlight on plates (light from top-left, always), 1 px darker bottom edge. No bevels, no noise, no rivets.
- Depth: plates sit at elevation 1; the hero card is inset (inner shadow from the top); the Brutality plate is the only raised level 2 object.
- Light: top, slightly left, everywhere.

**Layout** (1100 x 680; one character about 11 x 22 px)
```
+--------------------------------------------------------------------------------------------------+
| THE BUTCHER   [tag: Low End Rack  * ] < >  A|B   Browse  Init                  IN ||||  OUT |||| |
+--------------------------------------------------------------------------------------------------+
|+--------------------------------------------------------------+ +-------------------------------+|
||\ 1  2  3  4 | 5  6  7  8 | 9 10 11 12 |13 14 15 16         /| |  BRUTALITY  (macro)           ||
|| |.##.|.#...|..#..|.#.... waveform ......|........|.....   | | |        _.-"""-._              ||
|| |#####|##.##|#...#|##### drawn on the black card ###.##|    | |      .'   ,|     '.            ||
|| |....#|..#..|##.##|..#..                        ....       | |     |      \  scale   |           ||
|| |  leader lines to step numbers; muted steps stamped X      | |      '.  dial 0-10 .'            ||
||/ per-step on/off lives on the grid itself                  \| |        '-.______.-'             ||
|+--------------------------------------------------------------+ |  drives: Chance Drive Crush   ||
|                                                                  |  Cutoff  (rings on each)      ||
|+----------------+ +----------------+ +----------------+ +-------+------------------------------+||
|| CLEAVE         | | GRIND          | | FILTER         | | OUTPUT                               |||
|| slice+stutter  | | distort+crush  | | LP BP HP       | | mix + gain                           |||
|| Grid Chance    | | Mode Drive     | | Cutoff   Reso  | | Mix   Gain   meters                  |||
|| Swing Gate Rev | | Crush Rate Tone| |   ~curve~      | |                                      |||
|| Pitch Drop     | |                | |                | |                                      |||
|+----------------+ +----------------+ +----------------+ +--------------------------------------+|
+--------------------------------------------------------------------------------------------------+
```
Alignment: a 12-column grid with a 12 px gutter; hero card and module row share left and right edges, and the Brutality plate's bottom aligns to the hero card's bottom.

**The one memorable thing:** the slice grid drawn as a cut chart on the black price card. Zones are outlined, hairline leader lines run to step numbers, a muted step is crossed out by an ink-stamp X, and the playing slice flashes Cleave amber.

**What it refuses to be:** a dark neon plugin; a rivet and hazard-stripe skin; any picture of meat, animals or tools.

---

### C2. Kraft Parcel
Concept: the plugin as a wrapped parcel: warm kraft paper ground, black price-card modules tied with string, a rubber-stamp macro in the centre.

**Palette**
| Name | Hex | Role |
|---|---|---|
| Kraft | `#C9A877` | window ground |
| Parcel | `#A98458` | module backs, fold lines |
| Card black | `#1C1712` | hero display and module headers |
| Paper | `#F3EBDD` | numerals, trace, value plates |
| Stamp orange | `#D9480F` | the Brutality stamp and active states (orange-leaning, not red) |
| String ink | `#2A1F14` | legends on kraft |

**Type:** Zilla Slab 700 for module names (typewritten tag lettering) with Barlow 500 for values. Chosen because a parcel label is typed, not stamped in sans. Weights: 700 and 500.

**Shape and material:** radius 0 on cards with a punched round hole and a notched corner on tags; 3 px on controls; the stamp is a 50% circle with a rough-edged vector ring. Paper fold lines as 1 px lines, light from top. Flat, matte, no gradient.

**Layout**
```
+--------------------------------------------------------------------------------------------------+
| [tag: Low End Rack] < > A|B Browse Init   THE BUTCHER (typed)                    IN ||| OUT |||  |
+--------------------------------------------------------------------------------------------------+
|+------------------------------------------------------------------------------------------------+|
||  black card: waveform + slice grid, full width, steps along the bottom edge as tag numbers     ||
||                                                                                                ||
|+------------------------------------------------------------------------------------------------+|
| ~~~string~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |
| +-------------+ +-------------+  .---------------.  +-------------+ +-------------+              |
| | CLEAVE      | | GRIND       | /    BRUTALITY    \ | FILTER      | | OUTPUT      |              |
| | Grid Chance | | Mode Drive  | |  big stamp dial | | Type Cutoff | | Mix Gain    |              |
| | Swing Gate  | | Crush Rate  | \                 / | Reso        | | meters      |              |
| | Rev Pitch   | | Tone        |  '---------------'  |             | |             |              |
| +-------------+ +-------------+                      +-------------+ +-------------+              |
+--------------------------------------------------------------------------------------------------+
```
Alignment: centre axis; the dial sits on the vertical centre line with two modules either side, so left and right masses balance like a scale.

**The one memorable thing:** the macro is a rubber stamp. As Brutality rises, the stamp ring thickens and its ink gets rougher, and turning it to zero leaves a blank paper circle.

**What it refuses to be:** a cream and serif editorial page; a grunge skin.

---

### C3. Cut Chart (dark)
Concept: a dark tile-green walk-in room where the screen is a chart, with the hero at the centre and leader lines linking Brutality to the four controls it drives.

**Palette**
| Name | Hex | Role |
|---|---|---|
| Tile green | `#12302A` | window ground |
| Grout | `#1D4A40` | plates and 1 px dividers |
| Enamel white | `#EFF2E8` | legends, values, trace |
| Gold leaf | `#D8A94A` | Brutality and its leader lines (single accent) |
| Steel mist | `#8FA6A3` | secondary legends (6.6:1 on Tile) |
| Hero well | `#0B1C18` | display well |

**Type:** Libre Caslon Text 700 (`@fontsource/libre-caslon-text`) for module names in caps, a gold-fascia-lettering reference, with Barlow 500 for values.

**Shape and material:** 1:1 tile module of 22 px behind non-hero areas as 1 px grout lines at 6% contrast; plates 2 px radius, the hero 0; light from top, soft inner highlight; no glow.

**Layout**
```
+--------------------------------------------------------------------------------------------------+
| THE BUTCHER   [tag: Low End Rack] < > A|B  Browse Init                          IN ||| OUT |||    |
+--------------------------------------------------------------------------------------------------+
| +-----------+ +-------------------------------------------------------------+ +-----------------+|
| | CLEAVE    | |  hero: waveform + slice grid                                 | | GRIND           ||
| | slice     | |                                                              | | Mode Drive      ||
| | Grid      | |                                                              | | Crush Rate Tone ||
| | Chance <--+-+-- leader lines                                               | +-----------------+|
| | Swing     | +-------------------------------------------------------------+ +-----------------+|
| | Gate      | +-------------------------------------------------------------+ | FILTER          ||
| | Reverse   | |            BRUTALITY dial (centre, gold)                      | | Type Cutoff Reso||
| | Pitch drop| |                                                              | +-----------------+|
| +-----------+ +-------------------------------------------------------------+ +-----------------+|
|                                                                                | OUTPUT          ||
|                                                                                | Mix Gain meters ||
|                                                                                +-----------------+|
+--------------------------------------------------------------------------------------------------+
```
Alignment: three columns on a 12-column grid (3 / 6 / 3); leader lines are 1 px gold and run orthogonally.

**The one memorable thing:** pulling Brutality up draws gold leader lines out to Chance, Drive, Crush and Cutoff, so you see the macro's reach.

**What it refuses to be:** black with neon; a decorative green wash.

---

## 4. Genericness check (mandatory)

The no-thought direction for this brief:
- Palette: near-black `#0D0D10` with one acid-green or vermilion accent, grey knob caps.
- Type: Inter (or Roboto) everywhere, caps labels in tracked-out grey, one mono face for values.
- Layout: waveform across the top, a row of equal rounded cards (one per module) beneath, macro knob as one more card.
- Radius: 8 px on everything, soft drop shadow, glow on active arcs.

| Candidate | Palette | Type | Layout | Radius | Matches |
|---|---|---|---|---|---|
| C1 | no (cool light steel, green plate) | no | partly (hero then row of modules) | no (tiered 0/1/2/4/14) | 1, so pass |
| C2 | no (warm kraft) | no | no (centre dial among modules, string line) | no | 0 |
| C3 | partly (dark, but green with gold) | no | no (three columns, leader lines) | no | 0 to 1 |

Revisions made: C1 first used a black fascia with an orange accent, which is the cliche, so it became light steel with the dark confined to the hero card. C3 first used gold on near-black, so the ground became tile green and the accent gold limited to one element. C2 first used a serif plus terracotta on cream; that is on the anti-slop list, so the ground became saturated kraft, the type a slab, and the accent an orange stamp. Anti-slop scan: no uniform radius, no purple gradients, no all-caps eyebrow on each heading (caps only on module names), no middle-dot strings, no mono-for-everything, no numbered markers except the real 1-16 step sequence, no glow.

---

## 5. Decision

| Criterion | C1 Cold Counter | C2 Kraft Parcel | C3 Cut Chart |
|---|---|---|---|
| Fit to brief and user | 9: hero is waveform plus grid; four modules at a glance; legends readable | 7: the warm field fights producers' dark hosts | 8: dark suits the room; gold accent clear |
| Distinctiveness | 9 | 8 | 7 |
| Buildability (single HTML, vector) | 9 | 8 | 7: leader-line routing is fiddly |
| Risk | low-medium: light plugin on a dark screen | medium: could read as craft-fair | medium: dark plus gold edges toward luxury |

**Ranked:** 1. C1 Cold Counter. 2. C3 Cut Chart. Both are buildable in the single-file mock; C3 is the fallback if the human wants dark.

I chose C1. The reason is the legend problem: ink on steel is as readable as a plugin gets, and putting the only dark object in the window under the waveform makes the hero win by contrast, without a glow. The hero card carries the cut-chart leader lines. The Brutality enamel green plate is the only coloured mass. I will keep quiet: kraft (only on the preset tag and browser), the lane colours (small chips and arcs only), all texture (none), shadows (soft, one direction), and the stamps (only on muted steps and favourites). That leaves the cut chart and the plate to be remembered.

---

## 6. Direction rules (chosen direction, C1)

1. Fonts: only Barlow Condensed 600, Barlow 500 and Zilla Slab 600, embedded as base64 woff2 from `@fontsource/barlow-condensed`, `@fontsource/barlow` and `@fontsource/zilla-slab` (latin subset). Zilla Slab appears only on the preset name and tag numbers.
2. Minimum text 10 px; legends are 11 px caps at +0.06em; values use tabular numerals; no label is shown only on hover.
3. Radius tiers are fixed: 1 (step cells), 2 (controls, chips), 4 (module plates), 0 (hero card, with two notched top corners), 14 (Brutality plate, one chipped corner). No two tiers share a value.
4. The black price card `#12181D` exists only for the hero display and the meters. No other surface in the window is darker than `#44535D`.
5. The enamel green `#1D5C43` appears on one object: the Brutality plate. State colour excepted (focus, selected), no other green.
6. Each module header chip and its knob arcs use that module's lane colour and nothing else; lane colours never fill panels or backgrounds.
7. Kraft `#B58B57` appears only on the preset tag and the browser, with the tag shape (punched hole, notched corner, string).
8. Light comes from top-left: highlight on the top and left 1 px edges, shade on the bottom, with shadows from the elevation scale; no glow and no gradient except the plate's top-light sheen.
9. The slice grid is drawn as a cut chart: 1 px chalk zone lines, numbered leader lines, a muted step shown with an ink X stamp (not by dimming alone); the playing slice is Cleave-lit `#F2A93B`.
10. Brutality shows its reach: a thin ring in each target's lane colour appears on Chance, Drive, Crush and Cutoff, and its amount is readable as a number.
11. Every module has a plain subtitle beside its themed name ("CLEAVE slice and stutter", "GRIND distortion and crush"), and Mix is visible on the front page without scrolling or hovering.
12. No texture bitmaps, rivets, hazard stripes, cleaver or animal icons, red drips, or fills that are neon or glowing.
