#include "DryWet.h"



DryWet::DryWet(float defaultDryWetRadio)
{
    dryWetRatio = defaultDryWetRadio;
}


DryWet::~DryWet()
{
    
}


void DryWet::prepareToPlay(double sampleRate, int maxNumSamples)
{
    drySignal.setSize(2, maxNumSamples);
    drySignal.clear();
    
    updateState();
}


void DryWet::releaseResources()
{
    drySignal.setSize(0, 0);
}


void DryWet::copyDrySignal(const AudioBuffer<float> &buffer)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        drySignal.copyFrom(ch, 0, buffer, ch, 0, numSamples);
    }
}


void DryWet::mixSignals(AudioBuffer<float>& destinationBuffer)
{
    const auto numSamples = destinationBuffer.getNumSamples();
    const auto numChannels = destinationBuffer.getNumChannels();
    
    destinationBuffer.applyGain(wetGain);
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        drySignal.applyGain(ch, 0, numSamples, dryGain);
        destinationBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
    }
}


void DryWet::setDWRatio(float newValues)
{
    dryWetRatio = newValues;
    updateState();
}


void DryWet::updateState()
{
    dryGain = sqrt(1 - dryWetRatio);
    wetGain = sqrt(dryWetRatio);
}
