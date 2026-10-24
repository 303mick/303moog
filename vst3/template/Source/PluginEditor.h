#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

// Knob look drawn only from generated design tokens (see generated/Tokens.h).
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h, float pos,
                           float startAngle, float endAngle, juce::Slider&) override;
};

// A labelled rotary control bound to a parameter. Double-click resets to default.
class Knob : public juce::Component
{
public:
    Knob (juce::AudioProcessorValueTreeState&, const juce::String& paramId, const juce::String& label);
    void resized() override;
    void paint (juce::Graphics&) override;
    juce::Slider slider;

private:
    juce::String label;
    juce::RangedAudioParameter* param = nullptr;
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
};

class MeterBar : public juce::Component
{
public:
    void setLevel (float v) { level = v; repaint(); }
    void paint (juce::Graphics&) override;
private:
    float level = 0.0f;
};

class Editor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit Editor (Processor&);
    ~Editor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    Processor& proc;
    KnobLookAndFeel lnf;
    Knob gain, cutoff;
    MeterBar meterIn, meterOut;
    juce::ToggleButton bypass { "Bypass" };
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
