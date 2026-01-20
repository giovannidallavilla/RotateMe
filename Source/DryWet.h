#pragma once

#include <JuceHeader.h>

class DryWet
{
    public:
    DryWet(float defaultDryWetRatio = 0.5f);
    ~DryWet();
    
    void prepareToPlay(double SampleRate, int maxNumSamples);
    
    void releaseResources();
    
    void setDWRatio(float newValues);
    
    void copyDrySignal(const AudioBuffer<float>& buffer);
    
    void mixSignals(AudioBuffer<float>& destinationBuffer);
    
    private:
    float dryWetRatio;
    SmoothedValue<float, ValueSmoothingTypes::Linear> dryGain;
    SmoothedValue<float, ValueSmoothingTypes::Linear> wetGain;
    
    void updateState();
    
    AudioBuffer<float> drySignal;
  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
