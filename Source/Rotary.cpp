#include "Rotary.h"

#include <JuceHeader.h>

FractionalDelay::FractionalDelay() {}
FractionalDelay::~FractionalDelay() {}

void FractionalDelay::prepareToPlay(double newSampleRate, float maxNumSamples)
{
    sampleRate = newSampleRate;
    memorySize = roundToInt(<#const FloatType value#>)
}

void FractionalDelay::releaseResources()
{
    
}


