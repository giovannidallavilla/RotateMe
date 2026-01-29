#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"
#include "Layout.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;


class RotaryVisualizer : public Component, private Timer
{
    public:
    RotateMeAudioProcessor& processor;
    RotaryVisualizer(RotateMeAudioProcessor& p);
    ~RotaryVisualizer();
    
    void setRotationSpeed(float newValue);
    
    void paint(Graphics& g) override;
    
    private:
    void timerCallback() override;
    
    float speed;
    
    float currentAngle = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryVisualizer)
};


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
    Label dwLabel;
    Slider satSlider;
    Label satLabel;
    Slider satTypeSlider;
    Label satTypeLabel;
    Slider speedSlider;
    Label speedLabel;
    Slider brakeSlider;
    Label brakeLabel;
    void setupSliderRotary(Slider& slider, Label& label, int x, int y, int w, int h, float rotationWindow, String name);
    void setupSliderLinear(Slider& slider, Label& label, int x, int y, int w, int h, String name);
    
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
    
    MyLookAndFeel myTheme;
    PlateComponent commandPlate;
    PlateComponent presetPlate;
    HoleComponent upperHole;
    HoleComponent bottomHole;
    BrandComponent credits;
    
    Label presetBrowser;
    TextButton nextPreset { ">" };
    TextButton previousPreset { "<" };
    TextButton load { "Load" };
    TextButton save { "Save" };
    void setupPresetBrowser();
    void updatePresetBrowser();
    void loadPreset();
    void savePreset();
    
    int currentPreset = 0;
    
    RotaryVisualizer rotaryVisualizer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotateMeAudioProcessorEditor)
};
