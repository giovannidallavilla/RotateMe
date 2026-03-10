#pragma once

#include <JuceHeader.h>
#include "DryWet.h"
#include "Modulation.h"
#include "Saturation.h"
#include "Rotary.h"

class RotateMeAudioProcessor : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    RotateMeAudioProcessor();
    ~RotateMeAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    
    //==============================================================================
    const juce::String getName() const override {return JucePlugin_Name;};
    bool acceptsMidi() const override {return false;};
    bool producesMidi() const override {return false;};
    bool isMidiEffect() const override {return false;};
    double getTailLengthSeconds() const override {return 0.0;};
    int getNumPrograms() override {return 1;};
    int getCurrentProgram() override  {return 0;};
    void setCurrentProgram (int index) override {};
    const juce::String getProgramName (int index) override {return {};};
    void changeProgramName (int index, const juce::String& newName) override {};

    
    String getCurrentPresetName() const;
    int currentPresetIndex = 0;
    std::atomic<float> rotationSpeed { 0.0f };
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void addPreset(const juce::String name, const void* data, int sizeInBytes);
    void loadFactoryPreset(int index);

    
private:
    void parameterChanged(const String& parameterID, float newValue) override;
    void updateAllParameters();
    
    AudioProcessorValueTreeState parameters;
    
    AudioBuffer<float> pitchModulation;
    AudioBuffer<float> gainModulation;
    
    Rotary rotary;
    Saturation saturator;
    
    DryWet drywetter;
    
    LowFrequencyOscillator pitchLfo;
    LowFrequencyOscillator ampLfo;
    ParameterModulation timeModulation;
    ParameterModulation ampModulation;
    
    bool isBraked;
    
    juce::StringArray factoryPresetsNames;
    struct FactoryPresets
    {
        juce::String name;
        const char* data;
        int size;
    };
    std::vector<FactoryPresets> factoryPresets;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotateMeAudioProcessor)
};
