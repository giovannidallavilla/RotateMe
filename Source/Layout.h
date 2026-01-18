#pragma once


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
    
    constexpr int textBoxW = 74;
    constexpr int textBoxH = 21;
    
}
