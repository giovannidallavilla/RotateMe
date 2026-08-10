#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"
#include "DSPValues.h"

using namespace DSPValues;


// RotateMeAudioProcessor Class implementation
RotateMeAudioProcessor::RotateMeAudioProcessor()
: parameters(*this, nullptr, "RTY", Parameters::createParameterLayout()),
    drywetter(Parameters::defaultDryWet),
    rotary(),
    saturator(Parameters::defaultSatAmount),
    pitchLfo(defaultLfoFrequency, 0.0f),
    ampLfo(defaultLfoFrequency, 0.0f),
    timeModulation(Parameters::defaultPitchTime, 1.0),
    ampModulation(Parameters::defaultAmpValue, 1.0)
{
    dryWetParam    = parameters.getRawParameterValue(Parameters::nameDryWet);
    satAmountParam = parameters.getRawParameterValue(Parameters::nameSatAmount);
    modSpeedParam  = parameters.getRawParameterValue(Parameters::nameModSpeed);
    brakeParam     = parameters.getRawParameterValue(Parameters::nameBrake);
    satTypeParam   = parameters.getRawParameterValue(Parameters::nameSatType);

    factoryPresets = {
        { "Init", BinaryData::Init_xml, BinaryData::Init_xmlSize },
        { "Warm", BinaryData::Warm_xml, BinaryData::Warm_xmlSize },
        { "Bright", BinaryData::Bright_xml, BinaryData::Bright_xmlSize },
        { "Deep", BinaryData::Deep_xml, BinaryData::Deep_xmlSize }
    };
    
    for (const auto& p : factoryPresets)
    {
        factoryPresetsNames.add(p.name);
    }
}


RotateMeAudioProcessor::~RotateMeAudioProcessor() {}


void RotateMeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const int numChannels = getTotalNumInputChannels();
    const bool isStereo   = (numChannels == 2);
    
    rotary.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
    saturator.prepareToPlay(numChannels, samplesPerBlock, sampleRate);
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    timeModulation.prepareToPlay(sampleRate);
    ampModulation.prepareToPlay(sampleRate);
    
    pitchLfo.prepareToPlay(sampleRate);
    ampLfo.prepareToPlay(sampleRate);
    
    pitchModulation.setSize(2, samplesPerBlock);
    pitchModulation.clear();
    gainModulation.setSize(2, samplesPerBlock);
    gainModulation.clear();

    if (isStereo)
        pitchLfo.setStereoAngleDegrees(90.0f);
    else
        pitchLfo.setStereoAngleDegrees(0.0f);

    prevDryWet    = -1.0f;
    prevSatAmount = -1.0f;
    prevModSpeed  = -1.0f;
    prevBrake     = -1.0f;
    prevSatType   = -1.0f;
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
    
    updateAudioParameters();
    
    const auto numSamples = buffer.getNumSamples();
    const bool isStereo   = (buffer.getNumChannels() == 2);
    
    pitchLfo.generateBlock(pitchModulation, numSamples, isStereo);
    ampLfo.generateBlock(gainModulation,    numSamples, isStereo);
    timeModulation.processBlock(pitchModulation, numSamples);
    ampModulation.processBlock(gainModulation,   numSamples);
    
    drywetter.copyDrySignal(buffer);
    saturator.processBlock(buffer);
    rotary.processBlock(buffer, pitchModulation, gainModulation);
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
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
            prevDryWet    = -1.0f;
            prevSatAmount = -1.0f;
            prevModSpeed  = -1.0f;
            prevBrake     = -1.0f;
            prevSatType   = -1.0f;
        }
}


void RotateMeAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}


void RotateMeAudioProcessor::updateAudioParameters()
{
    if (dryWetParam != nullptr)
    {
        const float currentDryWet = dryWetParam->load();
        if (currentDryWet != prevDryWet)
        {
            drywetter.setDWRatio(currentDryWet);
            prevDryWet = currentDryWet;
        }
    }
    
    if (satAmountParam != nullptr)
    {
        const float currentSatAmount = satAmountParam->load();
        if (currentSatAmount != prevSatAmount)
        {
            saturator.setDrive(currentSatAmount);
            prevSatAmount = currentSatAmount;
        }
    }
    
    if (satTypeParam != nullptr)
    {
        const float currentSatType = satTypeParam->load();
        if (currentSatType != prevSatType)
        {
            int value = !static_cast<int>(currentSatType);
            saturator.setSatType(value);
            prevSatType = currentSatType;
        }
    }
    
    if (brakeParam != nullptr && modSpeedParam != nullptr)
    {
        const float currentBrake = brakeParam->load();
        const float currentModSpeed = modSpeedParam->load();
        
        bool brakeRequested = (currentBrake > 0.5f);
        
        if (brakeRequested != isBraked || currentModSpeed != prevModSpeed)
        {
            if (brakeRequested)
            {
                if (!isBraked)
                {
                    pitchLfo.saveCurrentFrequency();
                    ampLfo.saveCurrentFrequency();
                    pitchLfo.brake();
                    ampLfo.brake();
                    rotationSpeed.store(0.0f);
                    isBraked = true;
                }
            }
            else
            {
                isBraked = false;
                if (currentModSpeed == 0.0f)
                {
                    pitchLfo.setChorus();
                    rotary.setPitchDepth(chorusDepth);
                    ampLfo.setChorus();
                    rotationSpeed.store(48.0f);
                }
                else
                {
                    pitchLfo.setTremolo(6.0f);
                    rotary.setPitchDepth(tremoloDepth);
                    ampLfo.setTremolo(5.0f);
                    rotationSpeed.store(390.0f);
                }
            }
            prevBrake = currentBrake;
            prevModSpeed = currentModSpeed;
        }
    }
}


void RotateMeAudioProcessor::loadFactoryPreset(int index)
{
    int numPresets = static_cast<int>(factoryPresets.size());
    if (numPresets == 0)
        return;

    index = (index % numPresets + numPresets) % numPresets;

    if (isPositiveAndBelow(index, numPresets))
    {
        const auto preset = factoryPresets[index];
        if (preset.data != nullptr && preset.size > 0)
        {
            setStateInformation(preset.data, preset.size);
            currentPresetIndex = index;
        }
    }
}


void RotateMeAudioProcessor::addPreset(const juce::String name, const void *data, int sizeInBytes)
{
    FactoryPresets preset = { name, static_cast<const char*>(data), sizeInBytes };
    factoryPresets.push_back(preset);
    factoryPresetsNames.add(name);
    int index = (int)factoryPresets.size() - 1;
    loadFactoryPreset(index);
}


String RotateMeAudioProcessor::getCurrentPresetName() const
{
    return factoryPresetsNames[currentPresetIndex];
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RotateMeAudioProcessor();
}


