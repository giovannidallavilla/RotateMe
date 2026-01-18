#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"



RotateMeAudioProcessor::RotateMeAudioProcessor()
: parameters(*this, nullptr, "RTY", Parameters::createParameterLayout()),
    drywetter(Parameters::defaultDryWet),
    rotary(),
    saturator(Parameters::defaultSatAmount),
    pitchLfo(0.8f, 0.0f),
    ampLfo(0.8f, 0.0f),
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
    
    pitchLfo.prepareToPlay(sampleRate);
    ampLfo.prepareToPlay(sampleRate);
    
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
//    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
//    {
//        FloatVectorOperations::copy(buffer.getArrayOfWritePointers()[ch], pitchModulation.getArrayOfReadPointers()[ch], numSamples);
//    }
    drywetter.mixSignals(buffer);
}


bool RotateMeAudioProcessor::hasEditor() const
{
    return true;
}


juce::AudioProcessorEditor* RotateMeAudioProcessor::createEditor()
{
    return new RotateMeAudioProcessorEditor (*this, parameters);
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
    
    if (parameterID == Parameters::nameModSpeed && !isBraked)
    {
        if (newValue == 0)
        {
            pitchLfo.setChorus();
            ampLfo.setChorus();
        }
        else
        {
            pitchLfo.setTremolo(6.0);
            ampLfo.setTremolo(5.0);
        }
    }
    
    if (parameterID == Parameters::nameSatAmount)
    {
        saturator.setDrive(newValue);
    }
    
    if (parameterID == Parameters::nameBrake)
    {
        if (newValue)
        {
            pitchLfo.saveCurrentFrequency();
            ampLfo.saveCurrentFrequency();
            pitchLfo.brake();
            ampLfo.brake();
            isBraked = true;
        }
        else
        {
            pitchLfo.recoverLastFrequency();
            ampLfo.recoverLastFrequency();
            isBraked = false;
        }
    }
    
    if (parameterID == Parameters::nameSatType)
    {
        int value = newValue == 1 ? 0 : 1;
        saturator.setSatType(value);
    }
    
}


void RotateMeAudioProcessor::loadPreset(int index)
{
    index = jlimit(0, presets.size() - 1, index);
    currentPresetIndex = index;
}

String RotateMeAudioProcessor::getCurrentPresetName() const
{
    return presets[currentPresetIndex];
}



juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RotateMeAudioProcessor();
}


