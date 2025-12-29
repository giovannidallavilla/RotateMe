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
            auto satSample = sample > 0.0f ? tanh(sample * 1.2f * driveValue) : tanh(sample * 0.6f * driveValue);
            
            auto mix = jmap(driveValue, 0.1f, 12.0f, 0.0f, 0.1f);
            
            saturationData[ch][s] = sample * (1 - mix) + satSample * mix;
        }
        FloatVectorOperations::copy(data[ch], saturationData[ch], numSamples);
    }
}


void Saturation::setDrive(float newValue)
{
    newValue = jlimit(0.1f, 12.0f, newValue);
    drive.setTargetValue(newValue);
}
