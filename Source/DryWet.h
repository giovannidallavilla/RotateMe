#pragma once

#include <JuceHeader.h>

#define DEFAULT_DW_RATIO 0.5f

class DryWet
{
    public:
    DryWet(float defaultDryWetRatio = DEFAULT_DW_RATIO);
    ~DryWet();
    
    void prepareToPlay(double SampleRate, int maxNumSamples);
    
    void releaseResources();
    
    void setDWRatio(float newValues);
    
    void copyDrySignal(const AudioBuffer<float>& buffer);
    
    void mixSignals(AudioBuffer<float>& destinationBuffer);
    
    private:
    float dryWetRatio;
    float dryGain = 0.0f;
    float wetGain = 0.0f;
    
    void updateState();
    
    AudioBuffer<float> drySignal;
  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
