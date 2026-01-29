#include "Layout.h"



// PlateComponent Class implementation
PlateComponent::PlateComponent(Image& texture) : texture(texture) {}


PlateComponent::~PlateComponent() {}


void PlateComponent::paint(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.drawImage(texture, bounds);
    g.setColour(Colour::fromRGB(69, 50, 32));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 10.0f, 3.0f);
}



// HoleComponent Class implemenation
HoleComponent::HoleComponent() {}


HoleComponent::~HoleComponent() {}


void HoleComponent::paint(Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();
    float cornerSize = 18.0f;
    
    ColourGradient grad(
                        Colour::fromRGB(45, 46, 47),
                        0, 0,
                        Colour::fromRGB(45, 46, 47),
                        bounds.getWidth(), 0,
                        true
                        );
    grad.addColour(0.5f, Colour::fromRGB(64, 65, 66));
    g.setGradientFill(grad);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    g.setColour(Colour::fromRGB(69, 50, 31));
    g.drawRoundedRectangle(bounds.reduced(1.0f), cornerSize, 3.0f);
}



// BrandComponent Class implementation
BrandComponent::BrandComponent() {}


BrandComponent::~BrandComponent() {}


void BrandComponent::paint(Graphics &g)
{
    auto typeface = Typeface::createSystemTypefaceFor(BinaryData::Jauza_otf, BinaryData::Jauza_otfSize);
    Font jauzaFont(FontOptions(typeface).withHeight(48.0f));
    
    g.setColour(Colours::lightgrey);
    g.setFont(jauzaFont);
    g.drawText("RotateMe", cmdPlateX + cmdPlateW, padding_top - 340, 600, 600, juce::Justification::centred);
    
    g.setFont(FontOptions(15.0f));
    g.setColour(Colours::white);
    g.drawText("Developed by Giovanni Dalla Villa", presetPlateX + presetButtonW + 50, presetPlateY + presetLabelH + 20, textBoxW + 300, textBoxH + 50, juce::Justification::centred);
    g.drawText("at Laboratorio di Informatica Musicale", presetPlateX + presetButtonW + 50, presetPlateY + presetLabelH + 40, textBoxW + 300, textBoxH + 50, juce::Justification::centred);
}
