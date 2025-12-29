#pragma once

#include <JuceHeader.h>

class Saturation
{
    public:
    Saturation(const float defaultDrive);
    ~Saturation();
    
    void prepareToPlay(int numChannels, int numSamples, double sampleRate);
    
    void releaseResources();
    
    void processBlock(AudioBuffer<float>& buffer);
    
    void setDrive(float newValue);
    
    
    private:
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> drive;
    
    AudioBuffer<float> saturationBuffer;
};
