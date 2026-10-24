# Brief: The Butcher (VST3 plugin UI)

Slug: `the-butcher`   Date: 2026-10-24   Requested by: Mick ("let's test, create a UI for a VST plug called The Butcher")

## Product
**The Butcher** is a VST3 audio **effect** that chops and mangles whatever you feed it: a rhythmic slicer/stutter, a heavy distortion/bit-crusher, a filter, and a dry/wet mix, tied together by one big "Brutality" macro.

## Audience
Music producers working in bass music, hip hop, industrial and metal, mostly in Ableton Live on Windows. They want fast, aggressive, obviously-audible results and care how a plugin looks on a dark studio screen.

## Primary job
Chop up and wreck a sound in seconds, hear it change, dial how brutal it is, and save the result as a preset.

## Platform and size
- VST3, Windows + Ableton first; macOS later.
- Default window **1100 x 680** (aspect about 1.62:1), aspect-locked, resizable from 880 x 544 up to 1650 x 1020. Must fit a 1366 x 768 laptop with room for the host's window frame.
- Crisp at 100-200% display scaling. Everything vector.
- Minimum rendered text 10 px at default size.

## Framework
Single-file HTML mock (`design/the-butcher/mock/index.html`). A JUCE port is a later step and is not part of this run.

## Controls (assumed feature set, keep it to this)
| Module | Controls |
|---|---|
| **Cleave** (slice and stutter) | Grid (1/4, 1/8, 1/16, 1/32, 1/64, plus triplet and dotted), 16-step slice pattern (on/off per step), Chance, Swing, Gate shape (hard to soft), Reverse chance, Pitch drop |
| **Grind** (distortion and crush) | Mode (4 types), Drive, Crush (bit depth), Rate (downsample), Tone |
| **Filter** | Type (low-pass, band-pass, high-pass), Cutoff, Resonance |
| **Output** | Mix (dry/wet), Output gain, input and output meters |
| **Brutality** (macro) | One large control that drives Chance, Drive, Crush and Cutoff together |
| **Presets** | Name display, previous/next, browser with search and favourites, A/B compare, init |

## Display (the hero)
A live waveform of the incoming audio with the slice grid laid over it, so you see where the cuts fall and which steps are muted. Slices flash as they play. This is the one element the eye lands on first.

## References
None supplied. The reference analyst researches current best-in-class stutter, glitch and distortion effect plugins and what they do well and badly.

## Must keep
Nothing: this is a new plugin, no existing look to protect.

## Tone and constraints
- The name suggests a butcher's shop: use that world (steel, enamel, wood blocks, paper, tags, cold light) as inspiration for materials and shape language. **No gore, no blood, no depictions of violence or animals.** Craft and menace, not horror.
- Do not imitate any existing plugin, brand, logo or product design.
- Labels stay clear: a producer must understand every control without knowing the theme. Themed names are fine only if a plain subtitle sits beside them (for example "CLEAVE, slice and stutter").
- Plugin standards in `design/UI_AGENTS.md` section 5 apply in full.

## Assumptions (made without asking)
1. The Butcher is an **effect**, not an instrument, because the name implies cutting and wrecking existing sound.
2. The feature set above is a sensible, buildable core. The UX architect may restructure it but not add features.
3. Windows + Ableton Live is the target; the default window must fit a 1366 x 768 laptop.
4. A dark theme suits the audience and the studio setting. The art director may argue otherwise with evidence.
5. No light/dark toggle in v1; skins can come later as token swaps.
