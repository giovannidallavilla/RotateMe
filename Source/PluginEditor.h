#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"


typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;



class RotateMeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RotateMeAudioProcessorEditor (RotateMeAudioProcessor&, AudioProcessorValueTreeState&);
    ~RotateMeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RotateMeAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& valueTreeState;
    
    
    Slider dryWetSlider;
    Slider satSlider;
    Slider satTypeSlider;
    Slider speedSlider;
    Slider brakeSlider;
    void setupSlider(Slider& slider, int x, int y, int w, int h, float rotationWindow);
    
    
    MyLookAndFeel myTheme;
    
    
    std::unique_ptr<SliderAttachment> dryWetAttachment;
    std::unique_ptr<SliderAttachment> satAttachment;
    std::unique_ptr<SliderAttachment> satTypeAttachment;
    std::unique_ptr<SliderAttachment> speedAttachment;
    std::unique_ptr<SliderAttachment> brakeAttachment;
    
    Image woodTexture;
    Image commandPlateTexture;
    Image presetPlateTexture;
    Image generateWoodTexture();
    Image generatePlateTexture(int width, int height);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotateMeAudioProcessorEditor)
};
