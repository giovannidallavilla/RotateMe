#include "PluginProcessor.h"
#include "PluginEditor.h"

RotateMeAudioProcessor::RotateMeAudioProcessor() {}

RotateMeAudioProcessor::~RotateMeAudioProcessor() {}


void RotateMeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}

void RotateMeAudioProcessor::releaseResources() {}

void RotateMeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RotateMeAudioProcessor();
}
