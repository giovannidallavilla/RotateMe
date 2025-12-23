#include "DryWet.h"

#include <JuceHeader.h>

DryWet::DryWet(float defaultDryWetRadio)
{
    dryWetRatio = defaultDryWetRadio;
}

DryWet::~DryWet()
{
    
}

void DryWet::prepareToPlay(double sampleRate, int maxNumSamples)
{
    drySignal.setSize(
}
