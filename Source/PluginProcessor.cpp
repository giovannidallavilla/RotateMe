#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"



RotateMeAudioProcessor::RotateMeAudioProcessor()
: parameters(*this, nullptr, "RTY", Parameters::createParameterLayout()),
    drywetter(Parameters::defaultDryWet),
    rotary(),
    saturator(Parameters::defaultSatAmount),
    pitchLfo(Parameters::defaultModSpeed),
    ampLfo(Parameters::defaultModSpeed),
    timeModulation(Parameters::defaultPitchTime, 1.0),
    ampModulation(Parameters::defaultAmpValue, 1.0)

{
    Parameters::addGlobalListener(parameters, this);
}


RotateMeAudioProcessor::~RotateMeAudioProcessor()
{
    
}


void RotateMeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rotary.prepareToPlay(sampleRate, samplesPerBlock);
    saturator.prepareToPlay(2, samplesPerBlock, sampleRate);
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    timeModulation.prepareToPlay(sampleRate);
    ampModulation.prepareToPlay(sampleRate);
    
    pitchModulation.setSize(2, samplesPerBlock);
    pitchModulation.clear();
    gainModulation.setSize(2, samplesPerBlock);
    gainModulation.clear();
}


void RotateMeAudioProcessor::releaseResources()
{
    rotary.releaseResources();
    drywetter.releaseResources();
    
    pitchModulation.setSize(0, 0);
    gainModulation.setSize(0, 0);
}

void RotateMeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    const auto numSamples = buffer.getNumSamples();
    
    pitchLfo.generateBlock(pitchModulation, numSamples);
    ampLfo.generateBlock(gainModulation, numSamples);
    timeModulation.processBlock(pitchModulation, numSamples);
    ampModulation.processBlock(gainModulation, numSamples);
    
    drywetter.copyDrySignal(buffer);
    saturator.processBlock(buffer);
    rotary.processBlock(buffer, pitchModulation, gainModulation);
    drywetter.mixSignals(buffer);
}


bool RotateMeAudioProcessor::hasEditor() const
{
    return false;
}


juce::AudioProcessorEditor* RotateMeAudioProcessor::createEditor()
{
    return new RotateMeAudioProcessorEditor (*this);
}


void RotateMeAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}


void RotateMeAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}


void RotateMeAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == Parameters::nameDryWet)
    {
        drywetter.setDWRatio(newValue);
    }
    
    if (parameterID == Parameters::nameModSpeed)
    {
        if (newValue == 0)
        {
            pitchLfo.setFrequency(0.8f);
            ampLfo.setFrequency(0.8f);
        }
        else
        {
            pitchLfo.setFrequency(6.0f);
            ampLfo.setFrequency(5.0f);
        }
    }
    
    if (parameterID == Parameters::nameSatAmount)
    {
        saturator.setDrive(newValue);
    }
    
    if (parameterID == Parameters::nameBrake)
    {
        pitchLfo.setFrequency(0.0f);
        ampLfo.setFrequency(0.0f);
    }
    
}



juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RotateMeAudioProcessor();
}


