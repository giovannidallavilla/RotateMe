#pragma once
#include <JuceHeader.h>

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
    
    void drawLabel (Graphics&, Label&) override;
    
    private:
    
    int numTicks = 21;
    
    Path knobTop;
    Path knobPointer;
    Path sliderPointer;
    
    void generateShapes();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeel)
};






