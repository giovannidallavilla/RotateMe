#pragma once
#include <JuceHeader.h>

class Cabinet
{
    public:
    Cabinet();
    ~Cabinet();
    
    void prepareToPlay(const dsp::ProcessSpec& specs);
    
    void reset();
    
    void processBlock(AudioBuffer<float>& buffer);
    
    private:
    double sampleRate = 44100.0;
    int numChannels = 2;
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highPass;
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPass;
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> woodResonance;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cabinet)
};
