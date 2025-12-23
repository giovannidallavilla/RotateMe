#include "Modulation.h"

#include <JuceHeader.h>

LowFrequencyOscillator::LowFrequencyOscillator(float defaultFrequency)
{
    frequency.setCurrentAndTargetValue(defaultFrequency);
}

LowFrequencyOscillator::~LowFrequencyOscillator()
{
    
}

void LowFrequencyOscillator::prepareToPlay(double sampleRate)
{
    samplingPeriod = 1.0 / sampleRate;
    frequency.reset(sampleRate, 0.02);
}

void LowFrequencyOscillator::setFrequency(double newValue)
{
    jassert(newValue > 0.0);
    
    frequency.setTargetValue(newValue);
}

double LowFrequencyOscillator::processSample()
{
    auto sampleValue = 0.0;
    sampleValue = sin(currentPhase * MathConstants<double>::twoPi);
    
    const auto nextPhase = frequency.getNextValue() * samplingPeriod;
    currentPhase += nextPhase;
    currentPhase += static_cast<int>(currentPhase);
    return sampleValue;
}

void LowFrequencyOscillator::processBlock(AudioBuffer<float> &buffer, const int numSamples)
{
    const int numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    for (int s = 0; s < numSamples; s++)
    {
        const double sampleValue = processSample();
        
        for (int ch = 0; ch < numChannels; ch++)
        {
            data[ch][s] = sampleValue;
        }
    }
}


ParameterModulation::ParameterModulation(const double defaultParameter, const double defaultModAmount)
{
    parameter.setCurrentAndTargetValue(defaultParameter);
    modAmount.setCurrentAndTargetValue(defaultModAmount);
}

ParameterModulation::~ParameterModulation()
{
    
}

void ParameterModulation::prepareToPlay(double sampleRate)
{
    parameter.reset(sampleRate, 0.02);
    modAmount.reset(sampleRate, 0.02);
}

void ParameterModulation::processBlock(AudioBuffer<float> &buffer, const int numSamples)
{
    auto numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        FloatVectorOperations::add(data[ch], 1.0, numSamples);
        FloatVectorOperations::multiply(data[ch], 0.5, numSamples);
    }
    
    modAmount.applyGain(buffer, numSamples);
    
    if (parameter.isSmoothing())
    {
        for (int s = 0; s < numSamples; s++)
        {
            for (int ch = 0; ch < numChannels; ch++)
            {
                data[ch][s] = ch ? parameter.getCurrentValue() : parameter.getNextValue();
            }
        }
    }
    else
    {
        for (int ch = 0; ch < numChannels; ch++)
        {
            FloatVectorOperations::add(data[ch], parameter.getCurrentValue(), numSamples);
        }
    }
}

void ParameterModulation::setParameter(const double newValue)
{
    parameter.setTargetValue(newValue);
}

void ParameterModulation::setModAmount(const double newValue)
{
    modAmount.setTargetValue(newValue);
}

