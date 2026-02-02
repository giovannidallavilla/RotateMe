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
    
    void setSatType(int newValue);
    
    
    private:
    int saturationType = 0;
    
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> drive;
    
    inline float tube(float sample, float driveValue);
    
    inline float softHard(float sample, float threshold, float driveValue);
};
