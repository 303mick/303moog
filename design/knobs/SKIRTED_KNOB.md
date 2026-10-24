# Skirted bakelite knob (Mick's chosen knob style)

Reference supplied by Mick: black flared skirt, brushed-aluminium cap, pointer wedge, white indicator line running down the skirt.

Layers, on a 62-unit grid (centre 0,0), bottom to top:
1. Drop shadow: r19.5 black 50% offset (1.3, 2.2)
2. Skirt: r19.5, radial gradient #3d3e43 to #060607, stroke #020203; flare edge ring r17.6; soft top-left highlight arc
3. Upper body: r14.6, radial gradient #34353a to #050506
4. Aluminium cap: r10.2, linear gradient #f4f5f6 / #b9bcc1 / #e3e5e8 / #8d9096, stroke #3b3d41; 5 brushed rings (r2.2 to 7.8, 10% white or black)
5. Rotating: white line (#f4f2ea, width 2) from r12.6 to r18.4 on the skirt; dark wedge from r9.4 down to r1.2 on the cap

Implementations: `ui/index.html` (`knobSVG`), `vst3/template/Source/PluginEditor.cpp` (`KnobLookAndFeel::drawRotarySlider`). Keep them in step. Agents: when a design uses knobs and no other style is specified, use this one.
