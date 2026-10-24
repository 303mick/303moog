# Brief: REDLINE MODULAR legibility rework (UI Agents, VST3 Builder pipeline)

Slug: `redline-modular`   Date: 2026-10-24   Requested by: Mick ("stop butcher, carry on with redline")

## Product
REDLINE MODULAR: Moog-modular-style VST3 synth, Windows only, personal use. Existing UI: `ui/index.html` (single file, 26 modules in TOP/BOT rows on a dark walnut frame, patch cables, two-manual keyboard, pitch/mod wheels, presets, `window.Redline` API). Screenshot of the current state: `design/redline-modular/current-fit.png`.

## The problem (objective, from the audit)
At the 1366x768 fit size the whole rack is scaled down so labels render at **3 to 6 px** (module titles 6.4, knob labels 5, jack labels 4.5, scale numbers 3.3) against a 10 px floor. 95 audit errors. This is why Mick keeps saying he is not happy: it looks like a poster, not an instrument you can use.

## Goal
Every label legible (>= 10 px rendered, scale numerals >= 8 px as a documented exception) at 1366x768 and up, WITHOUT losing the modular look, the cables, or the ability to see the signal flow.

## Must keep (approved by Mick)
- Dark walnut frame, black brushed panels, cream lettering, red accents.
- The glossy skirted bakelite knob (`design/knobs/SKIRTED_KNOB.md`). Do not redesign knobs or jacks.
- All 26 modules, all parameters and IDs, the cable system, `window.Redline` API, presets, localStorage state key.

## Allowed to change
- How the rack is laid out and navigated: sections, tabs, horizontal pan/scroll, a mini-map, a focus/zoom on a module, a "voice" default view. The agents decide; judgment call, do not ask.
- Fit logic (stop shrinking everything to one screen). Default window size may change (target: fits 1366x768, opens at readable scale).

## Constraints
- Cables must still connect across the whole rack, including between modules not on screen at once (think: cable indicators at the edge, or auto-scroll while dragging).
- Minimum interactive target 24 px; keyboard accessible.
- The default "Basic Voice" patch must be fully visible and playable without scrolling at 1366x768.
- Framework: single-file HTML (update `ui/index.html` in place after QA passes); native JUCE port is a later step.

## Assumptions
1. Voice modules (oscillators, filter, envelopes, output) are what a player touches most; mods and effects are secondary.
2. A section-based layout plus smooth horizontal pan beats cramming.
3. No new features beyond navigation.
