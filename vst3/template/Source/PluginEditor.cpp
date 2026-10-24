#include "PluginEditor.h"
#include "../generated/Params.h"
#include "../generated/Tokens.h"

using namespace tokens;

//==============================================================================
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                                        float startAngle, float endAngle, juce::Slider& s)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (4.0f);
    const float r = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto c = bounds.getCentre();
    const float thick = r * 0.14f;
    const float arcR = r - thick * 0.5f;
    const float angle = startAngle + pos * (endAngle - startAngle);

    juce::Path track, value;
    track.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, endAngle, true);
    value.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, angle, true);
    g.setColour (color_track);
    g.strokePath (track, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour (s.isEnabled() ? color_accent_main : color_accent_dim);
    g.strokePath (value, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    const float capR = r - thick * 1.8f;
    g.setColour (color_bg_panel);
    g.fillEllipse (c.x - capR, c.y - capR, capR * 2.0f, capR * 2.0f);
    juce::Path tick;
    tick.addRoundedRectangle (-thick * 0.35f, -capR, thick * 0.7f, capR * 0.45f, thick * 0.35f);
    g.setColour (color_text_primary);
    g.fillPath (tick, juce::AffineTransform::rotation (angle).translated (c.x, c.y));

    if (s.hasKeyboardFocus (false))
    {
        g.setColour (color_accent_main);
        g.drawEllipse (c.x - r, c.y - r, r * 2.0f, r * 2.0f, 1.5f);
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
