#include "Rotary.h"

#include <JuceHeader.h>
#include "DSPValues.h"

using namespace DSPValues;


// Amplifier Class implementation
Amplifier::Amplifier(float defaultGain) {}


Amplifier::~Amplifier() {}


void Amplifier::prepareToPlay(float newSampleRate) {}


void Amplifier::releaseResources() {}


void Amplifier::processBlock(AudioBuffer<float> &buffer, AudioBuffer<float> &modulation)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    auto data = buffer.getArrayOfWritePointers();
    
    auto modulationData = modulation.getArrayOfReadPointers();
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        for (int s = 0; s < numSamples; s++)
        {
            auto lfo = modulationData[ch][s];
            auto gain = 1.0f + lfo * ampDepth;
            gain = jmax(0.0f, gain);
            
            data[ch][s] *= gain;
        }
    }
}


// Rotary Class implementation
Rotary::Rotary() : amp(1.0f)
{
    sampleRate = 1.0;
}


Rotary::~Rotary() {}


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
    delay.processBlock(buffer, timeModulation, pitchDepth);
    amp.processBlock(buffer, ampModulation);
}


void Rotary::setPitchDepth(float newValue)
{
    pitchDepth = newValue;
}


// PitchDelay Class implementation
PitchDelay::PitchDelay() {}


PitchDelay::~PitchDelay() {}


void PitchDelay::prepareToPlay(float newSampleRate, float maxNumSamples)
{
    for (int ch = 0; ch < 2; ch++)
    {
        smoothedDelay[ch].reset(sampleRate, defaultRamp);
    }
    
    writeIndex = 0;
    sampleRate = newSampleRate;
    memorySize = roundToInt(maxDelayTimeS * sampleRate) + maxNumSamples;
    
    memory.setSize(2, memorySize);
    memory.clear();
}


void PitchDelay::releaseResources()
{
    memorySize = 0;
    memory.setSize(0, memorySize);
}


void PitchDelay::processBlock(AudioBuffer<float> &buffer, AudioBuffer<float> &modulation, float pitchDepth)
{
    const int numChannels = buffer.getNumChannels();
        const int numSamples  = buffer.getNumSamples();

        auto bufferData = buffer.getArrayOfWritePointers();
        auto delayData = memory.getArrayOfWritePointers();
        auto modulationData = modulation.getArrayOfReadPointers();

        for (int s = 0; s < numSamples; ++s)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                float lfo = modulationData[ch][s];
                float delayMs = baseDelayMs + lfo * pitchDepth;
                delayMs = jlimit(3.0f, 9.0f, delayMs);
                float delaySamples = delayMs * 0.001f * sampleRate;

                smoothedDelay[ch].setTargetValue(delaySamples);
                float d = smoothedDelay[ch].getNextValue();
                float readIndex = writeIndex - d;
                if (readIndex < 0.0f)
                    readIndex += memorySize;
                int i0 = (int)readIndex;
                int i1 = (i0 + 1) % memorySize;
                float frac = readIndex - (float)i0;

                float y0 = delayData[ch][i0];
                float y1 = delayData[ch][i1];
                float out = y0 + frac * (y1 - y0);

                delayData[ch][writeIndex] = bufferData[ch][s];

                bufferData[ch][s] = out;
            }
            writeIndex = (writeIndex + 1) % memorySize;
        }
}
