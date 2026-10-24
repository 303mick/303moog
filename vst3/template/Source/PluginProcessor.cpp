#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../generated/Params.h"

Processor::Processor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "STATE", params::createLayout())
{
    pGain   = apvts.getRawParameterValue (params::ids::gain);
    pCutoff = apvts.getRawParameterValue (params::ids::cutoff);
    pBypass = apvts.getRawParameterValue (params::ids::bypass);
}

void Processor::prepareToPlay (double sampleRate, int)
{
    core.prepare (sampleRate, juce::jmax (1, getTotalNumOutputChannels()));
    core.setGainDb (pGain->load());
    core.setCutoffHz (pCutoff->load());
}

bool Processor::isBusesLayoutSupported (const BusesLayout& l) const
{
    const auto out = l.getMainOutputChannelSet();
    return (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo())
           && out == l.getMainInputChannelSet();
}

static float peakOf (const juce::AudioBuffer<float>& b)
{
    float p = 0.0f;
    for (int c = 0; c < b.getNumChannels(); ++c)
        p = juce::jmax (p, b.getMagnitude (c, 0, b.getNumSamples()));
    return p;
}

void Processor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (int c = getTotalNumInputChannels(); c < getTotalNumOutputChannels(); ++c)
        buffer.clear (c, 0, buffer.getNumSamples());

    inputPeak.store (peakOf (buffer), std::memory_order_relaxed);

    if (pBypass->load() < 0.5f)
    {
        core.setGainDb (pGain->load());
        core.setCutoffHz (pCutoff->load());
        core.process (buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    }

    outputPeak.store (peakOf (buffer), std::memory_order_relaxed);
}

juce::AudioProcessorEditor* Processor::createEditor() { return new Editor (*this); }

void Processor::getStateInformation (juce::MemoryBlock& dest)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, dest);
}

void Processor::setStateInformation (const void* data, int size)
{
    if (auto xml = getXmlFromBinary (data, size))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
