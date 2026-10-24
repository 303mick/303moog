# References: The Butcher (VST3 slicer / distortion / filter effect)

Date: 2026-10-24. Analyst: reference-analyst.

## Method and limits (read first)
- No reference images were supplied, so there is no pixel teardown and **no hex values were measured**. Section 1 is replaced by category teardowns from review text.
- Tags: **[measured]** = stated in a fetched page (source in brackets). **[inferred]** = my judgement, or recall of the product's look that no fetched page confirmed. Treat every look claim tagged [inferred] as unverified; the art director should open the products' own screenshot pages before copying any detail.
- Reviews rarely describe knob styling or colour. Where a fetch said so, I log "not described" rather than guess.
- Unreadable or thin: Infiltrator 2 Plugin Boutique page (features only, no layout); Stutter Edit 2 Attack Magazine page (no UI detail); Decapitator and Gross Beat knob/colour detail not in any fetched text.

---

## 1. Plugin teardowns (7)

### 1.1 Cableguys ShaperBox 3 (multi-effect, curve-driven)
Source: Sound On Sound review https://www.soundonsound.com/reviews/cableguys-shaperbox-3
- **Layout:** Shapers chained in a "Shaper Bar" along the top, signal left to right, reorder by drag. [measured]
- Each module has its own wet/dry Mix; up to 3 frequency bands with adjustable crossovers. [measured]
- Preset browser: 600+ presets, sorted by style, tag sub-categories, shows which Shapers each preset uses, favourites. [measured]
- Curve editor: pen tools for curves/steps/ramps, tempo-grid sync, Smooth control, copy curves between Shapers, sidechain waveform overlaid on source waveform. [measured]
- **Does well:** one conceptual idea (draw a shape, it is applied to a parameter over time) carried across every module; reviewer called the GUI "very slick" and listed no cons. [measured]
- **Weakness:** reordering outcomes are unpredictable until you know the modules. [measured]
- **Signature move:** the drawn curve over the live waveform is the interface. [inferred from description]
- **Per-module colour coding:** not described in the review. [measured: absent]

### 1.2 iZotope Stutter Edit 2 (stutter / performance)
Sources: Attack Magazine https://www.attackmagazine.com/news/izotope-stutter-edit-2-redesigned-with-more-effects-new-ui/ ; Synthtopia (listed, not opened)
- v2 redesigned look with "a logical workflow"; no specific layout detail in the text. [measured]
- Adds curve editor that can affect any setting, 12 pre-made curves; gestures are presets played from a MIDI keyboard; Auto mode auditions effects with no keyboard; output wet/dry plus limiter. [measured]
- **Does well:** the stutter is played, not programmed: a gesture on a key. Auto mode removes the "I have no controller" barrier. [measured + inferred]
- **Weakness:** depth sits behind a MIDI-keyboard mental model; a producer without a controller needs Auto mode to see anything. [inferred]
- **Signature move:** one-key gestures that fire a whole effect move. [measured]

### 1.3 Output Thermal (distortion, macro-first)
Source: MusicTech review https://musictech.com/reviews/plug-ins/output-thermal-review/
- Main page is minimal: one large circular X/Y pad with topographic contour lines is the hero; beside it a dry/wet fader and input/output level controls. The pad drives two macros, the only settings on that page. [measured]
- Advanced view: 3 stages in series, each with stage filter, 19 distortion algorithms, 9 stage effects; then 2 effects, then master filter and compressor with gain-reduction meter. [measured]
- Modulation: drag-and-drop handles, crescent-shaped amount indicators on destination parameters, unipolar/bipolar toggle; 2 envelopes, unlimited nodes. [measured]
- **Does well:** "clean, approachable main page suits beginners", depth one click away. This is the closest model for Brutality + hero display. [measured]
- **Weaknesses:** online manual does not explain architecture; preset browser has no scroll bar so categories are missed; envelopes cannot be MIDI-triggered. [measured]
- **Signature move:** the whole sound lives on one pad. [measured]

### 1.4 Sugar Bytes Effectrix 2 (sequenced effect lanes)
Source: MusicRadar https://www.musicradar.com/reviews/sugar-bytes-effectrix2-review
- Main area: timing divisions across the top, a column of colour-coded effect lanes down the side; modulation lane directly beneath the grid; parameter panel for the selected effect at the bottom. [measured]
- 14 effect modules (Looper A/B, Grain, Ring, Vinyl, Spectrum, Filter, Delay, Reverb); effects reorderable, each with own dry/wet. [measured]
- **Does well:** the step grid is the instrument; you paint effects into steps. [measured + inferred]
- **Weaknesses:** several preset types (effect presets vs modulation assignments) are confusing; randomiser cannot target specific elements. [measured]
- **Signature move:** colour-coded lane per effect so a glance tells you which effect is on which step. [measured]

### 1.5 Illformed Glitch 2 (multi-lane glitch sequencer)
Source: MusicRadar https://www.musicradar.com/reviews/tech/illformed-glitch-2-581491
- Each effect gets a horizontal lane; routing fixed top to bottom; drag blocks vertically between lanes. [measured]
- 128 Scenes per Program, each storing pattern, timing, effect settings; per-lane Randomise; Mutate makes slight variations of current settings. [measured]
- Reviewer: best interface of the glitch plugins compared; responsive. [measured]
- **Weaknesses:** presets, swing and module bypass missing from v1; no MIDI Learn; fixed routing. [measured]
- **Signature move:** Randomise + Mutate on every module (variation is one click, in two strengths). [measured]
- Relevance: Cleave needs swing and a Mutate-style button; Glitch 2's omission of swing is a listed complaint. [inferred]

### 1.6 Image-Line Gross Beat (time and volume envelopes)
Source: MusicRadar https://www.musicradar.com/reviews/tech/image-line-gross-beat-196008
- Two envelope panels: Volume (x = fixed four beats, y = volume, 100% top, points snap to eighths and jump rather than ramp) and Time (y = playback position, top = real time, down = up to two bars into the past; steep slope = spinback, stepped = cut-up). [measured]
- 36 pattern slots per processor per patch; trigger by scene click, MIDI note or CC; CC can control mix and "scratch" time offset. [measured]
- **Does well:** the axes mean something physical (a needle in the past), so shape equals sound. [measured + inferred]
- **Weaknesses:** "not for everyone"; only turntable-style volume/pitch, no real glitch effects. [measured]
- **Signature move:** drawing a time-travel line. [measured]

### 1.7 FabFilter Saturn 2 (multiband saturation) and Soundtoys Decapitator (single-knob saturation)
Source: MusicTech https://musictech.com/reviews/fabfilter-saturn-2-review/ ; Decapitator via Dubspot comparison page (listed, not opened)
- Saturn 2 hero: a frequency analyser, low left to high right; click the top of it to add a band (max 6); each band has own style, drive, feedback, frequency, dynamics, 3-stage EQ. [measured]
- Saturn 2 modulation: "+" at the bottom adds sources; drag from node; pulsing graphic confirms connection; amount window; colour around the destination shows modulation live. [measured]
- **Does well:** clear and uncluttered; modulation feedback is immediate. [measured]
- **Weaknesses:** some parameter legends only appear when the mouse is near them; reviewer took a while to find Mix; fascia "rather dark". [measured] These are the exact traps for a dark theme.
- Decapitator: large Drive knob with a stepped style selector and few other controls; one-gesture "turn it up until it hurts" workflow. [inferred from recall; not verified from a fetched page]
- **Signature move (Saturn):** drawing and modulating on the analyser itself. [measured]

### Also considered (not torn down)
- Infiltrator 2 / Texture (Devious Machines): 54 effects, up to 10 stacked, 32-step sequencer switching each module on/off, 4 macros, 1,500+ presets with macros pre-assigned. [measured, features only; layout unread: https://www.pluginboutique.com/product/2-Effects/53-Multi-Effect-/7627-Infiltrator-2]
  - Takeaway: presets arrive with macros already mapped, which is what makes a macro-led plugin feel instantly playable. [inferred]

### Cross-category lessons
- **Hero = the signal.** ShaperBox, Saturn, Gross Beat and Glitch all draw on top of real audio or a meaningful axis. Cleave's waveform-with-grid belongs in this family. [inferred]
- **Front page / deep page split** (Thermal) is what reviewers praise as beginner-friendly. [measured]
- **Colour as identity per lane/module** (Effectrix) beats labels alone for speed. [measured]
- **Biggest recurring complaints:** hidden controls and legends (Saturn), confusing preset layers (Effectrix, Thermal browser without scroll bar), missing swing/bypass/MIDI learn (Glitch 2). [measured]

---

## 2. Butcher-shop subject world (design material)
No gore. Use materials, signage, packaging, ritual objects.

### 2.1 What the sources show
- **Enamel sign (antique, early 20th c.):** green ground, white scrolling italic lettering, about 183 x 26 cm x 1 cm; oxidation and worn-away patches kept as character. [measured: The Hoarde listing https://www.thehoarde.com/product/rag-and-bone/antique-enamel-butchers-shop-sign-lamb-a-delicacy-early-20th-century-rab2414] (Listing text mentions an animal; take only the form: long horizontal panel, glossy fused ground, 1 cm thickness, chips at edges.)
- **Roast Fine Foods (Toronto, Gladstone Media):** 1950s diner meets clean modern; kraft brown, black, white with red accents; kraft card, wood, slate, chalkboard, concrete floor; tags, price cards, wooden and metal stamps, wrapped parcels with tags. [measured: https://www.creativeboom.com/work/a-tasty-new-identity-for-roast-fine-foods/]
- **Ellis Butchers (2026 identity, visie.io):** clean sans plus classic serif; high-contrast orange with deep forest green and jet black; kraft bags; black price cards with bold white type; gold fascia lettering; herringbone tile and brick; natural light, macro photography. [measured: https://visie.io/project/ellis-butchers-brand-identity-2026] (Its mascot motif is anthropomorphised meat cuts; the brief bans animal depiction, so skip.)
- Not fetched or thin: worldbranddesign.com and mutabor.de butcher identity pages (listed in search, not opened).

### 2.2 Transferable materials and devices
| Real-world object | What to take | Where it lands in the plugin |
|---|---|---|
| Brushed/stainless steel prep table, cold fluorescent light | cool grey-blue-tinted neutrals, single top light, hairline specular edge [inferred] | main panel ground, module plates |
| Glossy enamel sign, chipped edge | solid fused colour field, white lettering, small edge chips | the Brutality macro plate or preset name plate (one place only) |
| Kraft / butcher paper, string, wrapped parcel | warm mid-brown paper as a contrast surface | preset browser and tag cards, set against the cold steel |
| Hanging price tag / luggage tag on string | notched or punched tag shape, tied hole, handwritten-style number | preset tag, A/B slot, step labels |
| Rubber/ink stamp (circular or rectangular, imperfect) | single-colour stamped state marks | "muted" step mark, "favourite" mark |
| Chopping block (end-grain wood) | a physical plank and grid of end-grain squares | optional: the slice grid reads as a block, kept subtle |
| Cleaver, hooks, rail, scale | hooked rail along the top edge to hang modules; scale dial (0 to full sweep, with ticks) as meter/Brutality dial | meters and the macro dial. Hooks as pure geometry, no meat |
| Cut-chart diagrams (zones outlined and labelled on a silhouette) | numbered zone lines and leader lines as a diagram language | slice grid overlay: zones with leader-line labels. Use an abstract waveform, not an animal silhouette |
| Cold-room strip curtain, tile | vertical repeats, 1:1 tile grid with grout lines | background rhythm for non-hero areas |

### 2.3 Type material from the sources
- Enamel signage: scrolling script italic in white on a deep ground; shop fascias: gold-leaf serif capitals. [measured]
- Modern butcher brands: sans plus serif pairing, black price cards with bold white numerals. [measured]
- Stamp/label vernacular: condensed grotesque capitals, slab-serif tag numbers. [inferred]
- For a plugin: a condensed grotesque or slab for module names; tabular numerals for values. Name exact fonts in `direction.md`; I am not fixing them here.

---

## 3. Category clichés (the avoid list)
1. Near-black background with a single neon accent (acid green, vermilion, or cyan) on every plugin. [inferred]
2. Glowing rotary knobs with a gradient bar and a drop-shadow halo; glow used as the only state signal. [inferred]
3. Metal-textured "industrial" panels with rivets, screws and hazard stripes, heavy noise texture. [inferred; also banned by UI_AGENTS section 3]
4. Horror/gore: blood splatter, skulls, claws, red drips (explicitly banned by the brief).
5. Distressed "grunge" fonts used for body labels, which wrecks legibility at 10 px. [inferred]
6. A giant scrolling spectrum behind everything, so the hero is not clear. [inferred]
7. Tiny low-contrast labels in grey on black; legends that appear only on hover (Saturn criticism). [measured]
8. One big preset browser on top of everything or a browser without scroll cue (Thermal criticism). [measured]
9. Stacks of identical rounded rectangles for modules, one radius everywhere. [inferred; UI_AGENTS section 3]

## 4. Borrow / Avoid / Differentiate

### Borrow
- **Front page + deep page split**, X/Y or single big control up front (Thermal). Maps to Brutality + hero display up front, Grind detail behind.
- **Modulation shown on the destination**: ring or crescent around the target knob when Brutality drives it (Thermal crescent, Saturn colour ring). Directly serves "Brutality drives Chance, Drive, Crush, Cutoff".
- **Lane-colour identity for modules** (Effectrix): Cleave, Grind, Filter, Output each own one colour, used on the section header, knob arcs and the hero overlay for that module's effect.
- **Waveform with the control drawn on it** (ShaperBox, Saturn, Gross Beat): the slice grid sits on the live waveform; step toggles live on the grid, not in a separate row.
- **Randomise + Mutate pair** (Glitch 2) for the 16-step pattern; add **swing** (the thing Glitch 2 users miss).
- **Per-module wet/dry plus visible module bypass** (ShaperBox, Effectrix; bypass is a Glitch 2 complaint).
- **Preset browser** with tags, favourites, which-modules-used summary (ShaperBox), and a visible scroll bar (fixing Thermal).
- **Drag-visible connection pulse** (Saturn) when a source is dropped on a control.
- **Subject:** enamel-plate for the macro, kraft-paper tag for presets, stamp marks for state, scale-dial ticks for meters, cut-chart leader lines for slice labels.

### Avoid
- Everything in section 3, plus anything on the UI_AGENTS anti-slop list (one radius, neon on black, mono for every label, 01/02/03 on non-sequences, glow without job).
- Legends that only show on proximity; Mix hidden (Saturn). Mix must be findable in under 2 seconds.
- Themed names without plain subtitles; a producer should read "CLEAVE, slice and stutter" at a glance (brief rule).
- Literal meat, animals, blood, cleaver icons used as buttons.
- A hazard-stripe or rivet skin; steel texture as a bitmap.

### Differentiate (3 openings rooted in the subject)
1. **The cut chart as the hero overlay.** Butchers' diagrams divide a form into numbered zones joined to labels by leader lines. Render the slice grid that way over the waveform: zone outlines, hairline leader lines to step numbers, muted steps stamped out like crossed-out zones. No other effect plugin uses diagram language for its slices. Hero stays the waveform; the diagram is the overlay.
2. **Cold steel plus kraft tag duality.** Everything the producer plays is cold, tinted steel and enamel; everything they keep (presets, A/B, favourites) is a warm paper tag on string. Temperature encodes job: play vs store. Gives the preset browser an identity and a reason to be visually different from the modules.
3. **Brutality as an enamel plate with a scale dial.** The macro is a large dial with scale ticks (butcher's scales have a fixed arc and a heavy needle), set on a fused-colour enamel plate with a chipped edge. The macro gets its own material, which separates it from the knob family and satisfies one-hero spending.

## 5. Open questions and assumptions (not blocking)
1. **Colour:** no hex values were measured. Assumption: art director builds a tinted-steel neutral ramp and picks one accent plus at most one secondary; consider enamel-sign green or deep tag red-orange but not both, and verify neither reads as gore. Red must stay orange-leaning if used. [inferred]
2. **Hero stays the waveform** (per brief); the Brutality plate is the secondary focus, not a rival. Assumption: Brutality sits beside, not over, the display.
3. **Texture:** assumption that all material cues are vector (gradients, hairlines, chips as small paths), no bitmaps, per UI_AGENTS section 5.
4. **Fonts:** left open for the art director. Constraint from research: must hold at 10 px, need tabular numerals, at most 2 families.
5. **Brutality mapping visibility:** assumption that modulation rings on Chance, Drive, Crush, Cutoff are required, with an amount scale per target. This is a UX decision for ux-architect.
6. **Decapitator and Gross Beat visuals** were not verified from fetched text; the art director should view product pages before citing them in `direction.md`.
7. **Hooks:** the brief says no depictions of violence or animals. Assumption that a hook rail as plain geometry (a bent line) is acceptable; flag if the lead disagrees.
