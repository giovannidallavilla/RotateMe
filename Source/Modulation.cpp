#include "Modulation.h"
#include "DSPValues.h"
using namespace DSPValues;


// LowFrequencyOscillator Class implementation
LowFrequencyOscillator::LowFrequencyOscillator(float defaultFrequency, float defaultPhase)
{
    frequency.setCurrentAndTargetValue(defaultFrequency);
    phaseStateLeft = defaultPhase;
    phaseStateRight = defaultPhase + MathConstants<float>::halfPi;
}


LowFrequencyOscillator::~LowFrequencyOscillator() {}


void LowFrequencyOscillator::prepareToPlay(float newSampleRate)
{
    sampleRate = newSampleRate;
    samplePeriod = 1.0 / sampleRate;
    frequency.reset(sampleRate, unBrakeRamp);
}


void LowFrequencyOscillator::brake()
{
    frequency.reset(sampleRate, brakeRamp);
    frequency.setTargetValue(0.1);
}


void LowFrequencyOscillator::unBrake()
{
    frequency.reset(sampleRate, unBrakeRamp);
    frequency.setTargetValue(currentFrequency);
}


void LowFrequencyOscillator::setChorus()
{
    if (frequency.getTargetValue() >= 0.8f)
    {
        frequency.reset(sampleRate, chorusRamp);
    }
    frequency.setTargetValue(0.8);
    
    saveCurrentFrequency();
}


void LowFrequencyOscillator::setTremolo(float newValue)
{
    if (frequency.getTargetValue() >= 0.8f)
    {
        frequency.reset(sampleRate, tremoloRamp);
        frequency.setTargetValue(newValue);
        saveCurrentFrequency();
    }
}


void LowFrequencyOscillator::saveCurrentFrequency()
{
    currentFrequency = frequency.getTargetValue();
}


void LowFrequencyOscillator::recoverLastFrequency()
{
    frequency.reset(sampleRate, tremoloRamp);
    frequency.setTargetValue(currentFrequency);
}


void LowFrequencyOscillator::generateBlock(AudioBuffer<float> &buffer, const int maxNumSamples, bool stereo)
{
    isStereo = stereo;
    auto data = buffer.getArrayOfWritePointers();
    
    for (int s = 0; s < maxNumSamples; s++)
    {
        float freq = frequency.getNextValue();
        const float sampleLeft = generateSampleLeft(freq);
        
        data[0][s] = sampleLeft;
        
        if (isStereo)
        {
            data[1][s] = generateSampleRight(freq);
        }
        else
        {
            phaseStateRight += (freq * samplePeriod);
            phaseStateRight -= static_cast<int>(phaseStateRight);
        }
    }
}


float LowFrequencyOscillator::generateSampleLeft(float freq)
{
    auto sample = 0.0;
    
    sample = sin(phaseStateLeft * MathConstants<float>::twoPi);
    phaseStateLeft += (freq * samplePeriod);
    phaseStateLeft -= static_cast<int>(phaseStateLeft);
    
    return sample;
}


float LowFrequencyOscillator::generateSampleRight(float freq)
{
    auto sample = 0.0;
    sample = sin(phaseStateRight * MathConstants<float>::twoPi);
    
    phaseStateRight += (freq * samplePeriod);
    phaseStateRight -= static_cast<int>(phaseStateRight);
    
    return sample;
}


float LowFrequencyOscillator::getCurrentFrequency()
{
    return currentFrequency;
}

void LowFrequencyOscillator::setStereoAngleDegrees(float newValue)
{
    float angleNormalized = (newValue / 360.0f);
    stereoPhaseOffset = angleNormalized * MathConstants<float>::twoPi;
    
    phaseStateRight = phaseStateLeft + angleNormalized;
    phaseStateRight -= static_cast<int>(phaseStateRight);
}


// ParameterModulation Class implementation
ParameterModulation::ParameterModulation(const float defaultParameter, const float defaultAmount)
{
    parameter.setCurrentAndTargetValue(defaultParameter);
    amount.setCurrentAndTargetValue(defaultAmount);
}


ParameterModulation::~ParameterModulation() {}


void ParameterModulation::prepareToPlay(float sampleRate)
{
    parameter.reset(sampleRate, defaultRamp);
    amount.reset(sampleRate, defaultRamp);
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
