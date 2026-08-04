#pragma once
#include <JuceHeader.h>

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

    const Colour hornMetalLight = Colour::fromRGB (40, 42, 45);
    const Colour hornMetalDark = Colour::fromRGB (10, 11, 12);
    const Colour hornHubLight = Colour::fromRGB (100, 105, 110);
    const Colour hornHubDark = Colour::fromRGB (20, 22, 25);
}

namespace GUI
{
    struct WindowLayout
    {
        int w, h, padding_left, padding_top;
        int padding_right = w - padding_left;
        int padding_bottom = h - padding_top;
    };
    
    struct ComponentLayout
    {
        int x, y, w, h;
        float cornerSize, borderSize;
    };
    
    struct KnobLayout
    {
        int w, h;
        float cornerSize, borderSize, scale;
    };
    
    struct VisualizerLayout
    {
        int x, y, w, h;
        float openingSize, neckSize, hubSize;
    };
    
    struct TextBoxLayout
    {
        int w, h;
    };
    
    
    
    const WindowLayout window = {1280, 720, 140, 165};
    const auto percW = (window.w / 100);
    const auto percH = (window.h / 100);
    
    const ComponentLayout commandP = {window.padding_left, window.padding_top, percW * 40, percH * 80, 12.0f, 4.0f};
    const ComponentLayout presetP = {window.padding_right - 44 * (window.w / 100), window.padding_top + 290, percW * 50, percH * 25, 12.0f, 4.0f};
    const ComponentLayout upperH = {30, 20, window.w - 60, 25, 18.0f, 3.0f};
    const ComponentLayout lowerH = {30, window.h - 55, window.w - 60, 25, 18.0f, 3.0f};
    const ComponentLayout presetL = {0, 0, presetP.w - 200, 60};
    
    const VisualizerLayout visualizer = {presetP.x + 155, window.padding_top - 20, (int)(presetL.w * 0.75), (int)(presetL.w * 0.75), 50.0f, 7.0f, 24.0f};
    
    const KnobLayout cmdKnob = {120, 120, 0.25f, 2.0f, 0.85f};
    const KnobLayout presetB = {60, 60, 0, 0, 1.0f};
    
    const TextBoxLayout textBox = {74, 21};
}

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
