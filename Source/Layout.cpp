#include "Layout.h"



// PlateComponent Class implementation
PlateComponent::PlateComponent(Image& texture) : texture(texture) {}


PlateComponent::~PlateComponent() {}


void PlateComponent::paint(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.drawImage(texture, bounds);
    g.setColour(plateBorder);
    g.drawRoundedRectangle(bounds.reduced(1.0f), 10.0f, 3.0f);
}



// HoleComponent Class implemenation
HoleComponent::HoleComponent()
{
    cornerSize = holeCornerSize;
    borderThickness = holeBorderThickness;
}


HoleComponent::~HoleComponent() {}


void HoleComponent::paint(Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();
    
    ColourGradient grad(
                        holeBgBase,
                        0, 0,
                        holeBgBase,
                        bounds.getWidth(), 0,
                        true
                        );
    grad.addColour(0.5f, holeBgLight);
    g.setGradientFill(grad);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    g.setColour(holeBorder);
    g.drawRoundedRectangle(bounds.reduced(1.0f), cornerSize, borderThickness);
}



// BrandComponent Class implementation
BrandComponent::BrandComponent()
{
    auto typeface = Typeface::createSystemTypefaceFor(BinaryData::Jauza_otf, BinaryData::Jauza_otfSize);
    jauzaFont = Font(FontOptions(typeface).withHeight(48.0f));
}


BrandComponent::~BrandComponent() {}


void BrandComponent::paint(Graphics &g)
{
    g.setColour(Colours::lightgrey);
    g.setFont(jauzaFont);
    g.drawText("RotateMe", cmdPlateX + cmdPlateW, padding_top - 340, 600, 600, juce::Justification::centred);
    
    g.setFont(FontOptions(15.0f));
    g.setColour(Colours::white);
    g.drawText("Developed by Giovanni Dalla Villa", presetPlateX + presetButtonW + 50, presetPlateY + presetLabelH + 20, textBoxW + 300, textBoxH + 50, juce::Justification::centred);
    g.drawText("at Laboratorio di Informatica Musicale", presetPlateX + presetButtonW + 50, presetPlateY + presetLabelH + 40, textBoxW + 300, textBoxH + 50, juce::Justification::centred);
}
