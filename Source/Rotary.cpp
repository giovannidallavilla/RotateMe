#include "Rotary.h"

#include <JuceHeader.h>

#define MAX_DELAY_TIME 5.1f
#define BASE_DELAY_MS 6.0f
#define DEPTH_MS 2.5f
#define AMP_DEPTH 1.0f



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
        for (int s = 0; s < numSamples; s++)
        {
            auto lfo = modulationData[ch][s];
            auto gain = 1.0f + lfo * AMP_DEPTH;
            gain = jmax(0.0f, gain);
            
            data[ch][s] *= gain;
        }
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
    for (int ch = 0; ch < 2; ch++)
    {
        smoothedDelay[ch].reset(sampleRate, 0.02);
    }
    
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
    const int numChannels = buffer.getNumChannels();
        const int numSamples  = buffer.getNumSamples();

        auto bufferData = buffer.getArrayOfWritePointers();
        auto delayData = memory.getArrayOfWritePointers();
        auto modulationData = modulation.getArrayOfReadPointers();

        for (int s = 0; s < numSamples; ++s)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                // 1) LFO value (-1..+1)
                float lfo = modulationData[ch][s];

                // 2) Converti LFO in delay time (ms)
                //    baseDelayMs = 6 ms, depthMs = 2.5 ms (valori tipici Leslie)
                float delayMs = BASE_DELAY_MS + lfo * DEPTH_MS;

                // 3) Limita il delay a valori validi
                delayMs = jlimit(3.0f, 9.0f, delayMs);

                // 4) Converti in samples
                float delaySamples = delayMs * 0.001f * sampleRate;

                // 5) Calcola read index
                smoothedDelay[ch].setTargetValue(delaySamples);
                float d = smoothedDelay[ch].getNextValue();
                
                float readIndex = writeIndex - d;
                if (readIndex < 0.0f)
                    readIndex += memorySize;

                int i0 = (int)readIndex;
                int i1 = (i0 + 1) % memorySize;
                float frac = readIndex - (float)i0;

                // 6) Interpolazione lineare
                float y0 = delayData[ch][i0];
                float y1 = delayData[ch][i1];
                float out = y0 + frac * (y1 - y0);

                // 7) Scrivi nella delay line
                delayData[ch][writeIndex] = bufferData[ch][s];

                // 8) Output
                bufferData[ch][s] = out;
            }

            // 9) Avanza writeIndex
            writeIndex = (writeIndex + 1) % memorySize;
        }
    
}
