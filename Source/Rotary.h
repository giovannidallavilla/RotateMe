#pragma once

#include <JuceHeader.h>

class Rotary
{
    public:
    Rotary();
    ~Rotary();
    
    void prepareToPlay(double SampleRate, int maxNumSamples);
    
    void releaseResources();
    
    private:
};

class FractionalDelay
{
    public:
    
    FractionalDelay();
    ~FractionalDelay();
    
    void prepareToPlay(double newSampleRate, float maxNumSamples);
    
    void releaseResources();
    
    private:
    double sampleRate = 1.0;
    int memorySize = 0;
    
    AudioBuffer<float> delayMemory;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FractionalDelay)
};

