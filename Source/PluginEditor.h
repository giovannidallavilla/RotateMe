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
    //void setupButton(TextButton& button, int x, int y, int w, int h);
    //void setupLabel(Label& label, int x, int y, int w, int h);
    //void setupPresetBrowser();
    
    
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
    
    Label presetBrowser;
    TextButton nextPreset { ">" };
    TextButton previousPreset { "<" };
    TextButton load { "Load Preset" };
    TextButton save { "Save Preset" };
    void updatePresetBrowser();
    void loadPreset();
    void savePreset();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotateMeAudioProcessorEditor)
};
