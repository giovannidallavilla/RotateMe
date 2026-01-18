#pragma once

#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define KNOB_SCALE 0.85f



class MyLookAndFeel : public LookAndFeel_V4
{
    public:
    MyLookAndFeel();
    ~MyLookAndFeel();
    
    
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, Slider& slider) override;
    
    
    
    
    void setNumTicks(int newValue);
    
    
    void drawLinearSlider (juce::Graphics& g,
                           int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle style,
                           juce::Slider& slider) override;
    
    
    void drawButtonBackground(Graphics& g, Button& b, const Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    
    //void drawButtonText (Graphics&, TextButton&, bool highlighted, bool down) override;
    //
    //
    void drawLabel (Graphics&, Label&) override;
    
    
    private:
    
    int numTicks = 21;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeel)
};


