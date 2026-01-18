#include "Saturation.h"




Saturation::Saturation(const float defaultDrive)
{
    drive.setCurrentAndTargetValue(defaultDrive);
}


Saturation::~Saturation()
{
    
}


void Saturation::prepareToPlay(int numChannels, int numSamples, double sampleRate)
{
    saturationBuffer.setSize(numChannels, numSamples);
    drive.reset(sampleRate, 0.02);
}


void Saturation::processBlock(AudioBuffer<float> &buffer)
{
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();
    auto data = buffer.getArrayOfWritePointers();
    
    auto saturationData = saturationBuffer.getArrayOfWritePointers();
    for (int ch = 0; ch < numChannels; ch++)
    {
        for (int s = 0; s < numSamples; s++)
        {
            auto driveValue = drive.getNextValue();
            auto sample = data[ch][s];
            float satSample = (saturationType == 1) ? softHard(sample, 0.9f, driveValue) : tube(sample, driveValue);
            auto mix = jmap(driveValue, 0.1f, 12.0f, 0.0f, 0.1f);
                
            saturationData[ch][s] = sample * (1 - mix) + satSample * mix;
        }
        FloatVectorOperations::copy(data[ch], saturationData[ch], numSamples);
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
