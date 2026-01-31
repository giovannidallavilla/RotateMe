#pragma once
#include <JuceHeader.h>

namespace GUI
{
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr int padding_top = 165;
    constexpr int padding_left = 140;
    
    constexpr float holeCornerSize = 18.0f;
    constexpr float holeBorderThickness = 3.0f;
    
    constexpr int cmdPlateRatio_width = 40;
    constexpr int cmdPlateRatio_height = 80;
    constexpr int presetPlateRatio_width = 50;
    constexpr int presetPlateRatio_height = 25;
    
    constexpr float plateCornerRadius = 12.0f;
    constexpr float plateBorderThickness = 4.0f;
    
    constexpr int cmdPlateW = (width / 100) * cmdPlateRatio_width;
    constexpr int cmdPlateH = (height / 100) * cmdPlateRatio_height;
    constexpr int cmdPlateX = padding_left;
    constexpr int cmdPlateY = padding_top;
    
    constexpr int presetPlateW = (width / 100) * presetPlateRatio_width;
    constexpr int presetPlateH = (height / 100) * presetPlateRatio_height;
    constexpr int presetPlateX = width - (padding_left + presetPlateW - 78);
    constexpr int presetPlateY = padding_top + 290;
    
    constexpr int cmdKnobW = 120;
    constexpr int cmdKnobH = 120;
    constexpr float cmdKnobCorner = 0.25f;
    constexpr float cmdKnobThickness = 2.0f;
    constexpr int presetButtonW = 60;
    constexpr int presetButtonH = 60;
    constexpr int presetLabelW = presetPlateW - (2 * presetButtonW) - 80;
    constexpr int presetLabelH = 60;
    
    constexpr int textBoxW = 74;
    constexpr int textBoxH = 21;
    
    constexpr int holeW = width - 60;
    constexpr int holeH = 25;
    constexpr int upperHoleX = 30;
    constexpr int upperHoleY = 20;
    constexpr int bottomHoleX = 30;
    constexpr int bottomHoleY = height - holeH - 30;
    
    constexpr int rotaryVisualizerW = presetLabelW * 0.75;
    constexpr int rotaryVisualizerH = rotaryVisualizerW;
    constexpr int rotaryVisualizerX = presetPlateX + 95 + presetButtonW;
    constexpr int rotaryVisualizerY = padding_top - 20;
    
    
    constexpr float knobBorderWidth = 1.5f;
    constexpr float knobScale = 0.85f;
}



namespace MyColours
{
    const Colour plateBorder = Colour::fromRGB(69, 50, 32);
    const Colour holeBgBase = Colour::fromRGB(45, 46, 47);
    const Colour holeBgLight = Colour::fromRGB(64, 65, 66);
    const Colour holeBorder = Colour::fromRGB(69, 50, 31);
    
    const Colour knobBaseLight = Colour::fromRGB(35, 38, 40);
    const Colour knobBaseDark = Colour::fromRGB(26, 27, 27);
    const Colour knobPointerLight = Colour::fromRGB(220, 220, 220);
    const Colour knobPointerDark = Colour::fromRGB(140, 140, 140);
    
    const Colour sliderTrackLight = Colour::fromRGB(30, 30, 35);
    const Colour sliderTrackDark = Colour::fromRGB(26, 27, 30);
    const Colour sliderThumbBase = Colour::fromRGB(32, 34, 35);
    const Colour sliderThumbHighlight = Colour::fromRGB(180, 180, 180);
    const Colour sliderPointerLight = knobPointerLight;
    const Colour sliderPointerDark = Colour::fromRGB(180, 180, 180);
    
    const Colour buttonBaseLight = Colour::fromRGB(60, 60, 60);
    const Colour buttonBaseDark = Colour::fromRGB(29, 30, 31);
}


using namespace GUI;
using namespace MyColours;


class PlateComponent : public Component
{
    public:
    PlateComponent(Image& texture);
    ~PlateComponent();
    
    void paint(Graphics& g) override;
    
    private:
    Image& texture;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlateComponent)
};


class HoleComponent : public Component
{
    public:
    HoleComponent();
    ~HoleComponent();
    
    void paint(Graphics& g) override;
    
    private:
    float cornerSize = 1.0f;
    float borderThickness = 1.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HoleComponent)
};


class BrandComponent : public Component
{
    public:
    BrandComponent();
    ~BrandComponent();
    
    void paint(Graphics& g) override;
    
    private:
    Font jauzaFont;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrandComponent)
};
