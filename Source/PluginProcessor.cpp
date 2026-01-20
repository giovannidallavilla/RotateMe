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
    Parameters::addGlobalListener(parameters, this);
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
    
    updateAllParameters();
}


RotateMeAudioProcessor::~RotateMeAudioProcessor() {}


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
            updateAllParameters();
        }
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
            rotary.setPitchDepth(chorusDepth);
            ampLfo.setChorus();
            rotationSpeed.store(48.0f);
        }
        else
        {
            pitchLfo.setTremolo(6.0);
            rotary.setPitchDepth(tremoloDepth);
            ampLfo.setTremolo(5.0);
            rotationSpeed.store(390.0f);
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
            rotationSpeed.store(0.0f);
            isBraked = true;
        }
        else
        {
            pitchLfo.recoverLastFrequency();
            auto depth = (pitchLfo.getCurrentFrequency() == 6.0f) ? chorusDepth : tremoloDepth;
            rotary.setPitchDepth(depth);
            ampLfo.recoverLastFrequency();
            auto freq = (ampLfo.getCurrentFrequency() == 5.0f) ? 390.0f : 48.0f;
            rotationSpeed.store(freq);
            isBraked = false;
        }
    }
    
    if (parameterID == Parameters::nameSatType)
    {
        int value = newValue == 1 ? 0 : 1;
        saturator.setSatType(value);
    }
}


void RotateMeAudioProcessor::updateAllParameters()
{
    std::vector<String> params = { "BK", "DW", "MS", "SA", "ST" };
    for (auto& s : params)
    {
        auto p = parameters.getParameter(s);
        p->sendValueChangedMessageToListeners(p->getValue());
    }
}


void RotateMeAudioProcessor::loadFactoryPreset(int index)
{
    if (index < presetCount)
    {
        factoryPresets.resize(presetCount);
        factoryPresetsNames.removeRange(presetCount, factoryPresetsNames.size() - presetCount);
    }
    if (isPositiveAndBelow(index, (int)factoryPresets.size()))
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


