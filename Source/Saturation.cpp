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
        
        for (int ch = 0; ch < numChannels; ch++)
        {
            float in = data[ch][s];
            float out = 0.0f;
            
            if (saturationType == 0)
            {
                out = tube(in, driveValue);
            }
            else
            {
                out = softHard(in, 0.85f, driveValue);
            }
            
            data[ch][s] = out;
        }
    }
}


inline float Saturation::softHard(float sample, float threshold, float driveValue)
{
    float driven = sample * driveValue;
    
    if (driven > threshold)
    {
        return threshold + (driven - threshold) * 0.1f;
    }
    else if (driven < -threshold)
    {
        return -threshold + (driven + threshold) * 0.1f;
    }
    return driven;
}

inline float Saturation::tube(float sample, float driveValue)
{
    float driven = sample * driveValue;
    float offset = 0.1f;
    float x = driven + offset;
    float sat = std::tanh(x) - std::tanh(offset);
    return sat;
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
