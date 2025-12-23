#pragma once

#include <JuceHeader.h>

class LowFrequencyOscillator
{
    public:
    LowFrequencyOscillator(float defaultFrequency = 4.40);
    ~LowFrequencyOscillator();
    
    void prepareToPlay(double sampleRate);
    
    void processBlock(AudioBuffer<float>& buffer, const int numSamples);
    
    double processSample();
    
    void setFrequency(double newValue);
    
    
    private:
    double currentPhase = 0.0;
    double samplingPeriod = 1.0;
    
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> frequency;
    
};

class ParameterModulation
{
    public:
    ParameterModulation(const double defaultParameter = 0.0, const double defaultModAmont = 0.0);
    ~ParameterModulation();
    
    void prepareToPlay(double sampleRate);
    
    void setModAmount(const double newValue);
    
    void setParameter(const double newValue);
    
    void processBlock(AudioBuffer<float>& buffer, const int numSamples);
    
    
    private:
    SmoothedValue<double, ValueSmoothingTypes::Linear> parameter;
    SmoothedValue<double, ValueSmoothingTypes::Linear> modAmount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterModulation)
};
