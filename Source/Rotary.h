#pragma once

#include <JuceHeader.h>

class PitchDelay
{
    public:
    PitchDelay();
    ~PitchDelay();
    
    void prepareToPlay(float newSampleRate, float maxNumSamples);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& modulation, float pitchDepth);
    
    private:
    double sampleRate;
    int memorySize = 0;
    int writeIndex = 0;
    
    float oldSample[2] = {0.0, 0.0};
    
    AudioBuffer<float> memory;
    
    SmoothedValue<float> smoothedDelay[2];
    
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
    
    void setDelayDepth(float newValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Amplifier)
};


class Rotary
{
    public:
    Rotary();
    ~Rotary();
    
    void prepareToPlay(float newSampleRate, float maxNumSamples);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& timeModulation, AudioBuffer<float>& ampModulation);
    
    void setPitchDepth(float newValue);
    
    private:
    float sampleRate;
    float numSamples;
    float pitchDepth;
    
    PitchDelay delay;
    Amplifier amp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Rotary)
};
