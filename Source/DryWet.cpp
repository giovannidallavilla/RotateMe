#include "DryWet.h"
#include "DSPValues.h"
using namespace DSPValues;


// DryWet Class implementation
DryWet::DryWet(float defaultDryWetRadio)
{
    dryGain.setCurrentAndTargetValue(sqrt(1 - defaultDryWetRadio));
    wetGain.setCurrentAndTargetValue(sqrt(defaultDryWetRadio));
}


DryWet::~DryWet() {}


void DryWet::prepareToPlay(double sampleRate, int maxNumSamples)
{
    dryGain.reset(sampleRate, defaultRamp);
    wetGain.reset(sampleRate, defaultRamp);
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
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        destinationBuffer.applyGainRamp(ch, 0, numSamples, wetGain.getCurrentValue(), wetGain.getNextValue());
        destinationBuffer.addFromWithRamp(ch, 0, drySignal.getReadPointer(ch), numSamples, dryGain.getCurrentValue(), dryGain.getNextValue());
    }
}


void DryWet::setDWRatio(float newValues)
{
    dryWetRatio = newValues;
    updateState();
}


void DryWet::updateState()
{
    dryGain.setTargetValue(sqrt(1 - dryWetRatio));
    wetGain.setTargetValue(sqrt(dryWetRatio));
}
