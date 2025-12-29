#pragma once

#include <JuceHeader.h>

class LowFrequencyOscillator
{
    public:
    LowFrequencyOscillator(float defaultFrequency);
    ~LowFrequencyOscillator();
    
    void prepareToPlay(float sampleRate);
    
    void releaseResources();
    
    void generateBlock(AudioBuffer<float>& buffer, const int maxNumSamples);
    
    float generateSample();
    
    void setFrequency(float newValue);
    
    private:
    float samplePeriod = 1.0;
    float phaseState = 0.0;
    
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowFrequencyOscillator)
};


class ParameterModulation
{
    public:
    ParameterModulation(const float defaultParameter, const float defaultAmount);
    ~ParameterModulation();
    
    void prepareToPlay(float sampleRate);
    
    void processBlock(AudioBuffer<float>& buffer, const int maxNumSamples);
    
    void setAmount(const float newValue);
    
    void setParameter(const float newValue);
    
    private:
    SmoothedValue<float, ValueSmoothingTypes::Linear> parameter;
    SmoothedValue<float, ValueSmoothingTypes::Linear> amount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterModulation)
};
