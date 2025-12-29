#include "Modulation.h"



// LowFrequencyOscillator implementation
LowFrequencyOscillator::LowFrequencyOscillator(float defaultFrequency)
{
    frequency.setCurrentAndTargetValue(defaultFrequency);
}


LowFrequencyOscillator::~LowFrequencyOscillator()
{
    
}


void LowFrequencyOscillator::prepareToPlay(float sampleRate)
{
    samplePeriod = 1.0 / sampleRate;
    frequency.reset(sampleRate, 0.02);
}


void LowFrequencyOscillator::setFrequency(float newValue)
{
    // Value control to be implemented
    jassert(newValue > 0.0);
    
    frequency.setTargetValue(newValue);
}


void LowFrequencyOscillator::generateBlock(AudioBuffer<float> &buffer, const int maxNumSamples)
{
    const int numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    for (int s = 0; s < maxNumSamples; s++)
    {
        const float sample = generateSample();
        
        for (int ch = 0; ch < numChannels; ch++)
        {
            data[ch][s] = sample;
        }
    }
}


float LowFrequencyOscillator::generateSample()
{
    auto sample = 0.0;
    
    sample = sin(phaseState * MathConstants<float>::twoPi);
    
    
    phaseState += (frequency.getNextValue() * samplePeriod);
    phaseState -= static_cast<int>(phaseState);
    
    return sample;
}



// ParameterModulation implementation
ParameterModulation::ParameterModulation(const float defaultParameter, const float defaultAmount)
{
    parameter.setCurrentAndTargetValue(defaultParameter);
    amount.setCurrentAndTargetValue(defaultAmount);
}


ParameterModulation::~ParameterModulation()
{
    
}


void ParameterModulation::prepareToPlay(float sampleRate)
{
    parameter.reset(sampleRate, 0.02);
    amount.reset(sampleRate, 0.02);
}


void ParameterModulation::processBlock(AudioBuffer<float> &buffer, const int maxNumSamples)
{
    auto numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        FloatVectorOperations::add(data[ch], 1.0, maxNumSamples);
        FloatVectorOperations::multiply(data[ch], 0.5, maxNumSamples);
    }
    
    buffer.applyGain(amount.getCurrentValue());
    
    if (parameter.isSmoothing())
    {
        for (int s = 0; s < maxNumSamples; s++)
        {
            for (int ch = 0; ch < numChannels; ch++)
            {
                data[ch][s] += ch ? parameter.getCurrentValue() : parameter.getNextValue();
            }
        }
    }
    else
    {
        for (int ch = 0; ch < numChannels; ch++)
        {
            FloatVectorOperations::add(data[ch], parameter.getCurrentValue(), maxNumSamples);
        }
    }
}


void ParameterModulation::setAmount(const float newValue)
{
    amount.setTargetValue(newValue);
}


void ParameterModulation::setParameter(const float newValue)
{
    parameter.setTargetValue(newValue);
}
