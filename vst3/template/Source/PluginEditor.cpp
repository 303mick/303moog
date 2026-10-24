#include "PluginEditor.h"
#include "../generated/Params.h"
#include "../generated/Tokens.h"

using namespace tokens;

//==============================================================================
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                                        float startAngle, float endAngle, juce::Slider& s)
{
    // Skirted bakelite knob: flared black skirt, brushed-aluminium cap, pointer wedge,
    // white indicator line running down the skirt. Proportions follow the 62-unit design grid.
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (4.0f);
    const float R = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto c = bounds.getCentre();
    const float u = R / 24.6f;                         // 1 design unit in pixels
    const float angle = startAngle + pos * (endAngle - startAngle);

    // value track just outside the skirt
    const float trackR = 22.6f * u;
    juce::Path track, value;
    track.addCentredArc (c.x, c.y, trackR, trackR, 0.0f, startAngle, endAngle, true);
    value.addCentredArc (c.x, c.y, trackR, trackR, 0.0f, startAngle, angle, true);
    g.setColour (color_track);
    g.strokePath (track, juce::PathStrokeType (1.6f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour (s.isEnabled() ? color_accent_main : color_accent_dim);
    g.strokePath (value, juce::PathStrokeType (1.6f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto disc = [&] (float r) { return juce::Rectangle<float> (c.x - r * u, c.y - r * u, 2 * r * u, 2 * r * u); };

    g.setColour (juce::Colours::black.withAlpha (0.5f));                       // drop shadow
    g.fillEllipse (disc (19.5f).translated (1.3f * u, 2.2f * u));

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3d3e43), c.x - 6 * u, c.y - 8 * u,
                                             juce::Colour (0xff060607), c.x + 16 * u, c.y + 16 * u, true));
    g.fillEllipse (disc (19.5f));                                              // skirt
    g.setColour (juce::Colour (0xff020203));
    g.drawEllipse (disc (19.5f), 0.8f * u);
    g.setColour (juce::Colours::black.withAlpha (0.55f));
    g.drawEllipse (disc (17.6f), 0.6f * u);                                    // flare edge

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff34353a), c.x - 5 * u, c.y - 7 * u,
                                             juce::Colour (0xff050506), c.x + 14 * u, c.y + 14 * u, true));
    g.fillEllipse (disc (14.6f));                                              // upper body
    {   // specular sheen on the bakelite and a bright rim light on the skirt edge
        juce::Graphics::ScopedSaveState ss (g);
        g.addTransform (juce::AffineTransform::rotation (juce::degreesToRadians (-32.0f), c.x - 6.5f * u, c.y - 12.2f * u));
        g.setGradientFill (juce::ColourGradient (juce::Colours::white.withAlpha (0.65f), c.x - 6.5f * u, c.y - 15.0f * u,
                                                 juce::Colours::white.withAlpha (0.0f), c.x - 6.5f * u, c.y - 9.0f * u, false));
        g.fillEllipse (juce::Rectangle<float> (c.x - 16.0f * u, c.y - 16.8f * u, 19.0f * u, 9.2f * u));
    }
    g.setGradientFill (juce::ColourGradient (juce::Colours::white.withAlpha (0.8f), c.x - 13 * u, c.y - 13 * u,
                                             juce::Colour (0x779fb4ff), c.x + 13 * u, c.y + 13 * u, false));
    g.drawEllipse (disc (18.9f), 1.1f * u);

    // chrome-style aluminium: multi-stop sweep, bright rim, glint
    juce::ColourGradient al (juce::Colour (0xffffffff), c.x - 9 * u, c.y - 9 * u,
                             juce::Colour (0xff6e7077), c.x + 9 * u, c.y + 9 * u, false);
    al.addColour (0.18, juce::Colour (0xffcfd2d7));
    al.addColour (0.34, juce::Colour (0xff7c7f86));
    al.addColour (0.50, juce::Colour (0xfff2f4f6));
    al.addColour (0.68, juce::Colour (0xff9a9da4));
    al.addColour (0.84, juce::Colour (0xffe6e8eb));
    g.setGradientFill (al);
    g.fillEllipse (disc (10.2f));
    g.setColour (juce::Colour (0xff2c2e32));
    g.drawEllipse (disc (10.2f), 0.9f * u);
    for (float r : { 2.2f, 3.6f, 5.0f, 6.4f, 7.8f })                           // brushed rings
    {
        g.setColour (juce::Colours::white.withAlpha (0.10f));
        g.drawEllipse (disc (r), 0.45f * u);
    }
    juce::Path glint;
    glint.addCentredArc (c.x, c.y, 8.8f * u, 8.8f * u, 0.0f, juce::degreesToRadians (-80.0f), juce::degreesToRadians (-8.0f), true);
    g.setColour (juce::Colours::white.withAlpha (0.65f));
    g.strokePath (glint, juce::PathStrokeType (2.6f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // rotating parts: indicator line on the skirt and the pointer wedge on the cap
    const auto rot = juce::AffineTransform::rotation (angle).translated (c.x, c.y);
    juce::Path line, wedge;
    line.startNewSubPath (0.0f, -12.6f * u);
    line.lineTo (0.0f, -18.4f * u);
    g.setColour (juce::Colour (0xfff4f2ea));
    g.strokePath (line, juce::PathStrokeType (2.0f * u, juce::PathStrokeType::curved, juce::PathStrokeType::rounded), rot);
    wedge.startNewSubPath (-2.5f * u, -9.4f * u);
    wedge.lineTo (2.5f * u, -9.4f * u);
    wedge.lineTo (1.0f * u, -1.2f * u);
    wedge.lineTo (-1.0f * u, -1.2f * u);
    wedge.closeSubPath();
    g.setColour (juce::Colour (0xff1a1b1e));
    g.fillPath (wedge, rot);
    g.setColour (juce::Colours::black);
    g.strokePath (wedge, juce::PathStrokeType (0.4f * u), rot);

    if (s.hasKeyboardFocus (false))
    {
        g.setColour (color_accent_main);
        g.drawEllipse (disc (24.0f), 1.5f);
    }
}

//==============================================================================
Knob::Knob (juce::AudioProcessorValueTreeState& apvts, const juce::String& id, const juce::String& l)
    : label (l), param (apvts.getParameter (id)), attachment (apvts, id, slider)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::degreesToRadians (225.0f), juce::degreesToRadians (495.0f), true);
    slider.setDoubleClickReturnValue (true, apvts.getParameter (id)->convertFrom0to1 (apvts.getParameter (id)->getDefaultValue()));
    slider.setTitle (l);
    addAndMakeVisible (slider);
}

void Knob::resized()
{
    slider.setBounds (getLocalBounds().withTrimmedBottom ((int) (getHeight() * 0.28f)));
}

void Knob::paint (juce::Graphics& g)
{
    const float s = (float) getHeight() / size_knob;
    auto area = getLocalBounds().removeFromBottom ((int) (getHeight() * 0.28f));
    g.setColour (color_text_secondary);
    g.setFont (juce::FontOptions (size_text_label * s * 0.9f));
    g.drawText (label, area.removeFromTop (area.getHeight() / 2), juce::Justification::centred);
    g.setColour (color_text_primary);
    g.setFont (juce::FontOptions (size_text_value * s * 0.9f));
    g.drawText (param->getCurrentValueAsText() + " " + param->getLabel(), area, juce::Justification::centred);
}

void MeterBar::paint (juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setColour (color_track);
    g.fillRoundedRectangle (b, 2.0f);
    const float h = b.getHeight() * juce::jlimit (0.0f, 1.0f, level);
    g.setColour (level > 0.9f ? color_meter_hot : color_meter_ok);
    g.fillRoundedRectangle (b.withTop (b.getBottom() - h), 2.0f);
}

//==============================================================================
Editor::Editor (Processor& p)
    : AudioProcessorEditor (&p), proc (p),
      gain (p.apvts, params::ids::gain, "Gain"),
      cutoff (p.apvts, params::ids::cutoff, "Cutoff"),
      bypassAttachment (p.apvts, params::ids::bypass, bypass)
{
    gain.slider.setTextValueSuffix (" dB");
    gain.slider.setNumDecimalPlacesToDisplay (1);
    cutoff.slider.setTextValueSuffix (" Hz");
    cutoff.slider.setNumDecimalPlacesToDisplay (0);
    for (auto* c : std::initializer_list<juce::Component*> { &gain, &cutoff, &meterIn, &meterOut, &bypass })
        addAndMakeVisible (c);
    bypass.setColour (juce::ToggleButton::textColourId, color_text_primary);
    bypass.setColour (juce::ToggleButton::tickColourId, color_accent_main);
    bypass.setLookAndFeel (&lnf);

    setResizable (true, true);
    setResizeLimits ((int) size_min_width, (int) size_min_height, (int) size_max_width, (int) size_max_height);
    getConstrainer()->setFixedAspectRatio (size_base_width / size_base_height);
    setSize ((int) size_base_width, (int) size_base_height);
    for (auto* k : { &gain, &cutoff })
        k->slider.setLookAndFeel (&lnf);
    startTimerHz (30);
}

Editor::~Editor()
{
    for (auto* k : { &gain, &cutoff })
        k->slider.setLookAndFeel (nullptr);
    bypass.setLookAndFeel (nullptr);
}

void Editor::paint (juce::Graphics& g)
{
    g.fillAll (color_bg_base);
    const float s = (float) getWidth() / size_base_width;
    g.setColour (color_text_primary);
    g.setFont (juce::FontOptions (22.0f * s).withStyle ("Bold"));
    g.drawText ("TEMPLATE GAIN", juce::Rectangle<int> (0, (int) (8 * s), getWidth(), (int) (34 * s)), juce::Justification::centred);
}

void Editor::resized()
{
    const float s = (float) getWidth() / size_base_width;
    const int gut = (int) (size_gutter * s), top = (int) (56 * s);
    const int k = (int) (size_knob * s * 1.25f);
    const int cy = top + (getHeight() - top - gut - k) / 2;
    gain.setBounds   (gut + (int) (40 * s), cy, k, k);
    cutoff.setBounds (getWidth() - gut - (int) (40 * s) - k, cy, k, k);
    const int mw = (int) (10 * s), mh = (int) (150 * s);
    meterIn.setBounds  (getWidth() / 2 - mw - (int) (6 * s), cy, mw, mh);
    meterOut.setBounds (getWidth() / 2 + (int) (6 * s), cy, mw, mh);
    bypass.setBounds (getWidth() / 2 - (int) (40 * s), cy + mh + (int) (10 * s), (int) (80 * s), (int) (28 * s));
}

void Editor::timerCallback()
{
    meterIn.setLevel (proc.inputPeak.load (std::memory_order_relaxed));
    meterOut.setLevel (proc.outputPeak.load (std::memory_order_relaxed));
    gain.repaint();
    cutoff.repaint();
}
