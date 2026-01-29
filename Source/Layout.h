#pragma once
#include <JuceHeader.h>

namespace GUI
{
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr int padding_top = 165;
    constexpr int padding_left = 140;
    
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
    
    
    constexpr float borderWidth = 1.5f;
    constexpr float knobScale = 0.85f;
    
    
}

using namespace GUI;


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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HoleComponent)
};


class BrandComponent : public Component
{
    public:
    BrandComponent();
    ~BrandComponent();
    
    void paint(Graphics& g) override;
    
    private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrandComponent)
};
