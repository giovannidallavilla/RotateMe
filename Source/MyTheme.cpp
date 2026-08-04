#include "MyTheme.h"
#include "Layout.h"
#include "PluginProcessor.h"

using namespace GUI;
using namespace MyColours;


// MyLookAndFeel Class implementation
MyLookAndFeel::MyLookAndFeel()
{
    setNumTicks(21);
    generateShapes();
}


MyLookAndFeel::~MyLookAndFeel() {}


void MyLookAndFeel::generateShapes()
{
    const auto radius = 1.0f;
    const auto w = radius * 0.55f;
    const auto h = radius * 1.40f;
    const auto corner = h * 0.05f;
    knobTop.addRoundedRectangle(-w * 0.5f, -0.75f, w, h, corner);
    
    const auto pointerLength = radius * 0.42f;
    const float pointerWidth  = radius * 0.10f;
    knobPointer.addRoundedRectangle(-pointerWidth * 0.5f, -radius * 0.75f, pointerWidth, pointerLength, pointerWidth * 0.4f);
    
    sliderPointer.addRoundedRectangle(-0.05, -0.5, 0.5, 1.0, corner);
}


void MyLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height,
                                     float sliderPosProportional, float rotaryStartAngle,
                                     float rotaryEndAngle, Slider &slider)
{
    const float radius = (jmin(width, height) * 0.5f * cmdKnob.scale) - (cmdKnob.borderSize * 0.5f);
    const float cx = x + width * 0.5f;
    const float cy = y + height * 0.5f;
    const float kx = cx - radius;
    const float ky = cy - radius;
    const float kw = radius * 2.0f;
    
    g.setGradientFill(ColourGradient(
        knobBaseLight,
        kx, ky,
        knobBaseDark,
        kx, ky + kw,
        false
    ));
    g.fillEllipse(kx, ky, kw, kw);

    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(0.15f),
        cx - radius * 0.3f, cy - radius * 0.3f,
        Colours::transparentWhite,
        cx + radius * 0.3f, cy + radius * 0.3f,
        true
    ));
    g.fillEllipse(kx, ky, kw, kw);

    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(0.4f),
        kx, ky,
        Colours::black.withAlpha(0.4f),
        kx, ky + kw,
        false
    ));
    g.drawEllipse(kx, ky, kw, kw, 2.0f);
    
    float angle = (rotaryStartAngle + MathConstants<float>::halfPi) + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto top = knobTop;
    top.applyTransform(AffineTransform::scale(radius).rotated(angle).translated(cx, cy));
    g.setColour(juce::Colours::black.withAlpha(0.25f));
    g.fillPath(top);
    g.setGradientFill(juce::ColourGradient(
        knobBaseLight,
        cx, cy - radius,
        knobBaseDark,
        cx, cy + radius,
        false
    ));
    g.fillPath(top);
    g.setGradientFill(juce::ColourGradient(
        juce::Colours::white.withAlpha(0.35f),
        cx, cy - radius,
        juce::Colours::transparentWhite,
        cx, cy + radius,
        false
    ));
    g.fillPath(top);
    g.setColour(juce::Colours::black.withAlpha(0.45f));
    g.strokePath(top, juce::PathStrokeType(1.2f));

    auto ticks = (rotaryEndAngle - rotaryStartAngle) < (MathConstants<float>::pi) ? 2 : 21;
    setNumTicks(ticks);
    for (int i = 0; i < numTicks; ++i)
    {
        float t = (float)i / (float)(numTicks - 1);
        float a = rotaryStartAngle + t * (rotaryEndAngle - rotaryStartAngle);
        float thickness = 2.2f;
        float length = 10.0f;
        if (numTicks != 2)
        {
            thickness = (i % 10 == 0 ? 2.2f : 1.2f);
            length    = (i % 5 == 0 ? 10.0f : 6.0f);
        }
        float x1 = cx + std::cos(a) * (radius + length);
        float y1 = cy + std::sin(a) * (radius + length);
        float x2 = cx + std::cos(a) * (radius + 2.0f);
        float y2 = cy + std::sin(a) * (radius + 2.0f);
        
        auto colour = juce::Colours::white.withAlpha(0.9f);
        if (numTicks != 2)
        {
            colour = juce::Colours::white.withAlpha(i % 5 == 0 ? 0.9f : 0.6f);
        }
        g.setColour(colour);
        g.drawLine(x1, y1, x2, y2, thickness);
    }
    
    auto pointer = knobPointer;
    
    pointer.applyTransform(AffineTransform::scale(radius).rotated(angle).translated(cx, cy));

    g.setColour(Colours::black.withAlpha(0.25f));
    g.fillPath(pointer, AffineTransform::translation(1.5f, 1.5f));

    g.setGradientFill(ColourGradient(
        knobPointerLight,
        cx, cy - radius,
        knobPointerDark,
        cx, cy + radius,
        false
    ));
    g.fillPath(pointer);

    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(0.55f),
        cx, cy - radius * 0.6f,
        Colours::transparentWhite,
        cx, cy + radius * 0.6f,
        true
    ));
    g.fillPath(pointer);

    g.setColour(Colours::black.withAlpha(0.4f));
    g.strokePath(pointer, PathStrokeType(1.0f));
}


void MyLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos,
                                     float minSliderPos, float maxSliderPos,
                                     const juce::Slider::SliderStyle style,
                                     juce::Slider &slider)
{
    const float trackHeight = height * 0.20f;
    const float trackY = y + (height - trackHeight) * 0.5f;
    const float thumbW = width * 0.16f;
    const float thumbH = height * 0.65f;
    const float thumbX = sliderPos - (thumbW * 0.5f);
    const float thumbY = y + (height - thumbH) * 0.5f;
    const float thumbCorner = 3.0f;

    g.setGradientFill(juce::ColourGradient(
        sliderTrackLight,
        x, trackY,
        sliderTrackDark,
        x, trackY + trackHeight,
        false
    ));
    g.fillRect((float)x, trackY, (float)width, trackHeight);

    g.setColour(juce::Colour::fromRGB(40, 40, 40));
    g.drawRect((float)x, trackY, (float)width, trackHeight);

    g.setGradientFill(juce::ColourGradient(
        juce::Colours::white.withAlpha(0.1f),
        x, trackY + trackHeight * 0.25f,
        juce::Colours::transparentWhite,
        x, trackY + trackHeight * 0.75f,
        false
    ));
    
    if (abs(maxSliderPos - minSliderPos) < 1.0f)
    {
        minSliderPos = x;
        maxSliderPos = x + width;
    }
    
    setNumTicks(51);
    for (int i = 0; i < numTicks; ++i)
    {
        float length = 10.0f;
        length = (i % 10 == 0) ? 30.0f : 10.0f;
        float thickness = 1.0f;
        thickness = (i % 10 == 0) ? 1.5f : 1.0f;
        auto a = (float)i / (float)(numTicks - 1);
        float x1 = minSliderPos + a * (maxSliderPos - minSliderPos);
        float y1 = trackY - (length * 0.5);
        float y2 = trackY + (length * 0.5);
        
        g.setColour(Colours::white.withAlpha(0.8f));
        g.drawLine(x1, y1, x1, y2, thickness);
        
    }

    g.setColour(juce::Colours::black.withAlpha(0.25f));
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);

    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(0.4f),
        thumbX, thumbY,
        Colours::black.withAlpha(0.4f),
        thumbX, thumbY + thumbW,
        false
    ));
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);
    
    juce::ColourGradient thumbGrad(
                                   sliderThumbBase,
                                   thumbX, thumbY,
                                   sliderThumbBase,
                                   thumbX + thumbW, thumbY,
                                   false
                                   );
    thumbGrad.addColour(0.5f, sliderThumbHighlight);
    thumbGrad.addColour(0.25f, sliderThumbBase);
    thumbGrad.addColour(0.25f, sliderThumbBase);
    g.setGradientFill(thumbGrad);
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);

    g.setColour(juce::Colours::black.withAlpha(0.45f));
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);
    
    auto pW = thumbW * 0.1f;
    auto pH = thumbH * 0.9f;
    auto pointer = sliderPointer;
    
    pointer.applyTransform(AffineTransform::scale(pW, pH).translated(thumbX + thumbW * 0.5f, thumbY + thumbH * 0.5f));
    g.setColour(Colours::black.withAlpha(0.25f));
    g.fillPath(pointer, AffineTransform::translation(1.5f, 1.5f));
    
    g.setGradientFill(juce::ColourGradient(
                                       sliderPointerLight,
                                       0, thumbX,
                                       sliderPointerDark,
                                       0, thumbY + thumbH,
                                       true
                                       ));
    g.fillPath(pointer);
}


void MyLookAndFeel::drawButtonBackground(Graphics& g, Button& b, const Colour& backgroundColour,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown)
{
    int height = b.getHeight();
    float cornerSize = 7.0f;
    float thickness = 2.0f;
    auto buttonArea = b.getLocalBounds().toFloat().reduced(thickness / 2.0f);
    
    if (shouldDrawButtonAsDown)
    {
        buttonArea = buttonArea.translated(0.0f, 1.5f);
    }
    
    g.setGradientFill(ColourGradient(
                                     buttonBaseLight,
                                     0, 0,
                                     buttonBaseDark,
                                     0, height,
                                     true
                                     ));
    g.fillRoundedRectangle(buttonArea, cornerSize);
    
    g.setGradientFill(ColourGradient(
        Colours::white.withAlpha(0.6f),
        0, 0,
        Colours::black.withAlpha(0.1f),
        0, height,
        false
    ));
    g.drawRoundedRectangle(buttonArea, cornerSize, thickness);
}


void MyLookAndFeel::drawLabel (Graphics& g, Label& l)
{
    if (!l.isBeingEdited())
    {
        auto bgColour = l.findColour(Label::backgroundColourId);
        auto alpha = (l.isEnabled()) ? 1.0f : 0.5f;
        float thickness = 2.0f;
        
        if (bgColour.getAlpha() > 0)
        {
            const Rectangle<float> area = l.getLocalBounds().toFloat();
            float cornerSize = 7.0f;
            g.setGradientFill(ColourGradient(
                            bgColour.brighter(0.2f), 0, 0,
                            bgColour.darker(0.2f), area.getWidth() * 0.3f, area.getHeight() * 0.5f,
                            true));
                        
            g.fillRoundedRectangle(area, cornerSize);
            
            g.setColour(Colours::white.withAlpha(0.1f));
            g.drawRoundedRectangle(area, cornerSize, thickness);
        }
        
        g.setColour (l.findColour (juce::Label::textColourId).withAlpha (alpha));
        g.setFont (getLabelFont (l));
        
        auto textArea = getLabelBorderSize (l).subtractedFrom (l.getLocalBounds());
                
        g.drawText (l.getText(), textArea.toFloat(),
                    l.getJustificationType(),
                    true);
    }
}


void MyLookAndFeel::setNumTicks(int newValue)
{
    numTicks = newValue;
}
