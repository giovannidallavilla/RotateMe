#include "Cabinet.h"

Cabinet::Cabinet()
{
    
}

Cabinet::~Cabinet()
{
    
}

void Cabinet::prepareToPlay(const dsp::ProcessSpec &specs)
{
    sampleRate  = specs.sampleRate;
    numChannels = static_cast<int>(specs.numChannels);
    
    *highPass.state     = *dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 60.0f,   0.707f);
    *lowPass.state      = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,  7500.0f, 0.707f);
    *woodResonance.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 350.0f, 1.0f, 1.33f);
    
    highPass.prepare(specs);
    lowPass.prepare(specs);
    woodResonance.prepare(specs);
}


void Cabinet::reset()
{
    highPass.reset();
    lowPass.reset();
    woodResonance.reset();
}

void Cabinet::processBlock(AudioBuffer<float> &buffer)
{
    if (numChannels == 1)
    {
        auto monoBlock = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
        dsp::ProcessContextReplacing<float> ctx(monoBlock);
        
        highPass.process(ctx);
        woodResonance.process(ctx);
        lowPass.process(ctx);
    }
    else
    {
        dsp::AudioBlock<float> block(buffer);
        dsp::ProcessContextReplacing<float> ctx(block);
        
        highPass.process(ctx);
        woodResonance.process(ctx);
        lowPass.process(ctx);
    }
}
