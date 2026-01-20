#pragma once

#include <JuceHeader.h>

class LowFrequencyOscillator
{
    public:
    LowFrequencyOscillator(float defaultFrequency, float defaultPhase);
    ~LowFrequencyOscillator();
    
    void prepareToPlay(float newSampleRate);
    
    void releaseResources();
    
    void generateBlock(AudioBuffer<float>& buffer, const int maxNumSamples);
    
    float generateSampleLeft(float freq);
    float generateSampleRight(float freq);
    
    void brake();
    
    void unBrake();
    
    void setChorus();
    
    void setTremolo(float newValue);
    
    void saveCurrentFrequency();
    
    void recoverLastFrequency();
    
    float getCurrentFrequency();
    
    private:
    float sampleRate;
    float samplePeriod = 1.0;
    float phaseStateLeft = 0.0;
    float currentFrequency = 1.0;
    float phaseStateRight = MathConstants<float>::halfPi;
    
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
