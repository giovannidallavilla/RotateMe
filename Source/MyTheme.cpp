#pragma once
#include "MyTheme.h"
#include "Layout.h"
using namespace GUI;


MyLookAndFeel::MyLookAndFeel()
{
        numTicks = 21;
}

MyLookAndFeel::~MyLookAndFeel()
{
    
}

void MyLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height,
                                     float sliderPosProportional, float rotaryStartAngle,
                                     float rotaryEndAngle, Slider &slider)
{
    const float radius = (jmin(width, height) * 0.5f * KNOB_SCALE) - (BORDER_WIDTH * 0.5f);
    const float cx = x + width * 0.5f;
    const float cy = y + height * 0.5f;
    const float kx = cx - radius;
    const float ky = cy - radius;
    const float kw = radius * 2.0f;
    
    
    g.setGradientFill(ColourGradient(
        Colour::fromRGB(35, 38, 40),
        kx, ky,
        Colour::fromRGB(26, 27, 27),
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
    
    
    const float topW = radius * 0.55f;
    const float topH = radius * 1.40f;
    const float corner = topH * 0.05f;
    juce::Path top;

    top.addRoundedRectangle(-topW * 0.5f, -radius * 0.75f, topW, topH, corner);
    float angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
    top.applyTransform(juce::AffineTransform::rotation(angle).translated(cx, cy));
    g.setColour(juce::Colours::black.withAlpha(0.25f));
    g.fillPath(top, juce::AffineTransform::translation(1.5f, 1.5f));

    g.setGradientFill(juce::ColourGradient(
        juce::Colour::fromRGB(35, 38, 40),
        cx, cy - radius,
        juce::Colour::fromRGB(26, 27, 27),
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

    auto ticks = (rotaryEndAngle/MathConstants<float>::pi) == 0.25 ? 2 : 21;
    setNumTicks(ticks);
    for (int i = 0; i < numTicks; ++i)
    {
        float t = (float)i / (float)(numTicks - 1);
        float a = rotaryStartAngle + t * (rotaryEndAngle - rotaryStartAngle) - MathConstants<float>::halfPi;
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
    
    
    const float pointerLength = radius * 0.42f;
    const float pointerWidth  = radius * 0.10f;
    Path pointer;
    pointer.addRoundedRectangle(-pointerWidth * 0.5f, -radius * 0.75f, pointerWidth, pointerLength, pointerWidth * 0.4f);

    
    angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
    pointer.applyTransform(AffineTransform::rotation(angle).translated(cx, cy));


    g.setColour(Colours::black.withAlpha(0.25f));
    g.fillPath(pointer, AffineTransform::translation(1.5f, 1.5f));


    g.setGradientFill(ColourGradient(
        Colour::fromRGB(220, 220, 220),
        cx, cy - radius,
        Colour::fromRGB(140, 140, 140),
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
    const float trackX = x;
    const float trackWidth = width;
    const float thumbRadius = height * 0.45f;
    const float thumbW = width * 0.16f;
    const float thumbH = height * 0.65f;
    const float thumbX = sliderPos - thumbRadius * 0.5f;
    const float thumbY = y + (height - thumbH) * 0.5f;
    const float thumbCorner = 3.0f;

    
    g.setGradientFill(juce::ColourGradient(
        juce::Colour::fromRGB(30, 30, 35),
        x, trackY,
        juce::Colour::fromRGB(26, 27, 30),
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
    
    
    setNumTicks(50);
    for (int i = 0; i < numTicks; ++i)
    {
        float t = (float)i / (float)(numTicks - 1);
        float a = trackX + t * (trackWidth);
        float thickness = 1.0f;
        float length = 10.0f;
        thickness = (i % 10 == 0 ? 1.5f : 1.0f);
        length    = (i % 10 == 0 ? 30.0f : 10.0f);
        float x1 = trackX + a;
        float y1 = trackY - (length * 0.5f);
        float x2 = trackX + a;
        float y2 = trackY + (length * 0.5f);
        
        auto colour = juce::Colours::white.withAlpha(0.9f);
        if (numTicks != 2)
        {
            colour = juce::Colours::white.withAlpha(i % 5 == 0 ? 0.9f : 0.6f);
        }
        g.setColour(colour);
        g.drawLine(x1, y1, x2, y2, thickness);
    }
    g.fillRect((float)x, trackY, (float)width, trackHeight);


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
                                   juce::Colour::fromRGB(32, 34, 35),
                                   thumbX, thumbY,
                                   juce::Colour::fromRGB(32, 34, 35),
                                   thumbX + thumbW, thumbY,
                                   false
                                   );
    thumbGrad.addColour(0.5f, juce::Colour::fromRGB(180, 180, 180));
    thumbGrad.addColour(0.25f, juce::Colour::fromRGB(30, 30, 30));
    thumbGrad.addColour(0.25f, juce::Colour::fromRGB(30, 30, 30));
    g.setGradientFill(thumbGrad);
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);

    
    g.setColour(juce::Colours::black.withAlpha(0.45f));
    g.fillRoundedRectangle(thumbX, thumbY, thumbW, thumbH, thumbCorner);
    
    
    auto pointerW = thumbW * 0.075f;
    auto pointerH = thumbH;
    auto pointerCorner = thumbCorner * 0.45f;
    auto pointerX = thumbX + (thumbW / 2) - (pointerW / 2);
    auto pointerY = thumbY;
    Path pointer;
    pointer.addRoundedRectangle(pointerX, pointerY, pointerW, pointerH, pointerCorner);
    g.setColour(Colours::black.withAlpha(0.25f));
    g.fillPath(pointer, AffineTransform::translation(1.5f, 1.5f));
    
    g.setGradientFill(juce::ColourGradient(
                                       juce::Colour::fromRGB(220, 220, 220),
                                       pointerX, pointerY,
                                       juce::Colour::fromRGB(180, 180, 180),
                                       pointerX, pointerY + pointerH,
                                       true
                                       ));
    g.fillPath(pointer);
}


void MyLookAndFeel::drawButtonBackground(Graphics& g, Button& b, const Colour& backgroundColour,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown)
{
    int width = b.getWidth();
    int height = b.getHeight();
    float cornerSize = 7.0f;
    float thickness = 2.0f;
    auto buttonArea = b.getLocalBounds().toFloat().reduced(thickness / 2.0f);
    if (shouldDrawButtonAsDown)
    {
        buttonArea = buttonArea.translated(0.0f, 1.5f);
    }
    
    
    g.setGradientFill(ColourGradient(
                                     Colour::fromRGB(60, 60, 60),
                                     0, 0,
                                     Colour::fromRGB(29, 30, 31),
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


//void MyLookAndFeel::drawButtonText (Graphics&, TextButton&, bool highlighted, bool down)
//{
//
//}
//
//
void MyLookAndFeel::drawLabel (Graphics& g, Label& l)
{
    if (!l.isBeingEdited())
    {
        auto alpha = (l.isEnabled()) ? 1.0f : 0.5f;
        const Rectangle<float> area = l.getLocalBounds().toFloat();
        float cornerSize = 7.0f;
        float thickness = 2.0f;
        
        
        g.setGradientFill(ColourGradient(
                                         Colour::fromRGB(80, 80, 80),
                                         0, 0,
                                         Colour::fromRGB(40, 40, 40),
                                         0, height,
                                         true
                                         ));
        g.fillRoundedRectangle(area, cornerSize);
        
        
        g.setGradientFill(ColourGradient(
            Colours::white.withAlpha(0.6f),
            0, 0,
            Colours::black.withAlpha(0.1f),
            0, height,
            false
        ));
        g.drawRoundedRectangle(area, cornerSize, thickness);
        
        
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

