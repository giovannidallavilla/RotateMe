#include "Rotary.h"

#include <JuceHeader.h>

#define MAX_DELAY_TIME 5.1f



// Amplifier implementation
Amplifier::Amplifier(float defaultGain)
{
    
}


Amplifier::~Amplifier()
{
    
}


void Amplifier::prepareToPlay(float newSampleRate)
{
    
}


void Amplifier::releaseResources()
{
    
}


void Amplifier::processBlock(AudioBuffer<float> &buffer, AudioBuffer<float> &modulation)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    auto modulationData = modulation.getArrayOfReadPointers();
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        FloatVectorOperations::multiply(data[ch], modulationData[ch], numSamples);
    }
}



// Rotary implementation
Rotary::Rotary() : amp(1.0f)
{
    
}


Rotary::~Rotary()
{
    
}


void Rotary::prepareToPlay(float newSampleRate, float maxNumSamples)
{
    sampleRate = newSampleRate;
    numSamples = maxNumSamples;
    
    delay.prepareToPlay(sampleRate, numSamples);
    amp.prepareToPlay(sampleRate);
}


void Rotary::releaseResources()
{
    delay.releaseResources();
    amp.releaseResources();
}


void Rotary::processBlock(AudioBuffer<float> &buffer, AudioBuffer<float> &timeModulation, AudioBuffer<float> &ampModulation)
{
    delay.processBlock(buffer, timeModulation);
    amp.processBlock(buffer, ampModulation);
}



// PitchDelay implementation
PitchDelay::PitchDelay()
{
    
}


PitchDelay::~PitchDelay()
{
    
}


void PitchDelay::prepareToPlay(float newSampleRate, float maxNumSamples)
{
    sampleRate = newSampleRate;
    memorySize = roundToInt(MAX_DELAY_TIME * sampleRate) + maxNumSamples;
    
    memory.setSize(2, memorySize);
    memory.clear();
}


void PitchDelay::releaseResources()
{
    memorySize = 0;
    memory.setSize(0, memorySize);
}


void PitchDelay::processBlock(AudioBuffer<float> &buffer, AudioBuffer<float> &modulation)
{
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();
    auto bufferData = buffer.getArrayOfWritePointers();
    
    auto delayData = memory.getArrayOfWritePointers();
    
    auto modulationData = modulation.getArrayOfWritePointers();
    auto modulationChannels = modulation.getNumChannels();
    
    for (int s = 0; s < numSamples; s++)
    {
        for (int ch = 0; ch < numChannels; ch++)
        {
            auto time = modulationData[jmin(ch, modulationChannels - 1)][s];
            auto readIndex = writeIndex - (time * sampleRate);
            
            auto integrer = static_cast<int>(readIndex);
            auto fractional = readIndex - integrer;
            auto alpha = fractional / (2.0 - fractional);
            
            auto A = (integrer + memorySize) % memorySize;
            auto B = (A + 1) % memorySize;
            
            delayData[ch][writeIndex] = bufferData[ch][s];
            
            auto sample = alpha * (delayData[ch][B] - oldSample[ch]) + delayData[ch][A];
            oldSample[ch] = sample;
            
            bufferData[ch][s] = sample;
        }
        ++writeIndex %= memorySize;
    }
    
}
