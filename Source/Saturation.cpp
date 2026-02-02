#include "Saturation.h"
#include "DSPValues.h"
using namespace DSPValues;


// Saturation Class implementation
Saturation::Saturation(const float defaultDrive)
{
    drive.setCurrentAndTargetValue(defaultDrive);
}


Saturation::~Saturation() {}


void Saturation::prepareToPlay(int numChannels, int numSamples, double sampleRate)
{
    drive.reset(sampleRate, defaultRamp);
}


void Saturation::processBlock(AudioBuffer<float> &buffer)
{
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();
    auto data = buffer.getArrayOfWritePointers();
    auto* leftCh = data[0];
    auto* rightCh = (numChannels > 1) ? data[1] : nullptr;
    
    for (int s = 0; s < numSamples; s++)
    {
        auto driveValue = drive.getNextValue();
        auto mix = jmap(driveValue, 0.1f, 12.0f, 0.0f, 0.1f);
        
        auto sampleLeft = leftCh[s];
        auto satSampleLeft = (saturationType == 1) ? softHard(sampleLeft, 0.9f, driveValue) : tube(sampleLeft, driveValue);
        leftCh[s] = sampleLeft * (1 - mix) + satSampleLeft * mix;
        
        if (rightCh != nullptr)
        {
            auto sampleRight = rightCh[s];
            auto satSampleRight = (saturationType == 1) ? softHard(sampleRight, 0.9f, driveValue) : tube(sampleRight, driveValue);
            rightCh[s] = sampleRight * (1 - mix) + satSampleRight * mix;
        }
    }
}


inline float Saturation::softHard(float sample, float threshold, float driveValue)
{
    if (sample > threshold)
    {
        return threshold + (sample - threshold) * 0.1f;
    }
    
    if (sample < -threshold)
    {
        return threshold + (sample + threshold) * 0.1f;
    }
    return sample;
}

inline float Saturation::tube(float sample, float driveValue)
{
    auto satSample = sample > 0.0f ? tanh(sample * 1.2f * driveValue) : tanh(sample * 0.6f * driveValue);
    return satSample;
}


void Saturation::setDrive(float newValue)
{
    newValue = jlimit(0.1f, 12.0f, newValue);
    drive.setTargetValue(newValue);
}


void Saturation::setSatType(int newValue)
{
    saturationType = newValue;
}
