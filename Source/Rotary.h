#pragma once

#include <JuceHeader.h>

class PitchDelay
{
    public:
    PitchDelay();
    ~PitchDelay();
    
    void prepareToPlay(float newSampleRate, float maxNumSamples);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& modulation);
    
    
    private:
    double sampleRate;
    int memorySize = 0;
    int writeIndex = 0;
    
    float oldSample[2] = {0.0, 0.0};
    
    AudioBuffer<float> memory;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchDelay)
};



class Amplifier
{
    public:
    Amplifier(float defaultGain);
    ~Amplifier();
    
    void prepareToPlay(float newSampleRate);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& modulation);
    
    
    private:
    // SmoothedValue<float, ValueSmoothingTypes::Linear> gain;
};



class Rotary
{
    public:
    Rotary();
    ~Rotary();
    
    void prepareToPlay(float newSampleRate, float maxNumSamples);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& timeModulation, AudioBuffer<float>& ampModulation);
    
    private:
    float sampleRate;
    float numSamples;
    
    PitchDelay delay;
    Amplifier amp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Rotary)
};
