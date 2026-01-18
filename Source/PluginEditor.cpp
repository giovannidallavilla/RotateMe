#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"
#include "Layout.h"

using namespace GUI;

//==============================================================================
RotateMeAudioProcessorEditor::RotateMeAudioProcessorEditor (RotateMeAudioProcessor& p, AudioProcessorValueTreeState& valueTreeState)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(valueTreeState)
{
    setSize (width, height);
    
    
    setupSlider(satSlider, cmdPlateX, cmdPlateY, cmdKnobW, cmdKnobH, cmdKnobCorner + 0.5f);
    setupSlider(satTypeSlider, cmdPlateX + cmdPlateW - 280, cmdPlateY, cmdKnobW, cmdKnobH, cmdKnobCorner);
    setupSlider(speedSlider, cmdPlateX, cmdPlateY + 180, cmdKnobW, cmdKnobH, cmdKnobCorner);
    setupSlider(brakeSlider, cmdPlateX + cmdPlateW - 280, cmdPlateY + 180, cmdKnobW, cmdKnobH, cmdKnobCorner);
    
    dryWetSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(dryWetSlider);
    dryWetSlider.setBounds(cmdPlateX - 15, cmdPlateY + 350, 350, 50);
    
    
    dryWetAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameDryWet , dryWetSlider));
    satAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameSatAmount , satSlider));
    satTypeAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameSatType, satTypeSlider));
    speedAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameModSpeed , speedSlider));
    brakeAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameBrake , brakeSlider));
    
    
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(nextPreset);
    addAndMakeVisible(previousPreset);
    addAndMakeVisible(load);
    addAndMakeVisible(save);
    
    presetBrowser.setJustificationType(Justification::centred);
    presetBrowser.setText(audioProcessor.getCurrentPresetName(), dontSendNotification);
    
    nextPreset.onClick = [this]
    {
        audioProcessor.loadPreset(audioProcessor.currentPresetIndex - 1);
        updatePresetBrowser();
    };
    previousPreset.onClick = [this]
    {
        audioProcessor.loadPreset(audioProcessor.currentPresetIndex + 1);
        updatePresetBrowser();
    };
    
    
    load.onClick = [this] { loadPreset(); };
    save.onClick = [this] { savePreset(); };
    
    
    this->setLookAndFeel(&myTheme);
    
    woodTexture = generateWoodTexture();
    commandPlateTexture = generatePlateTexture(cmdPlateW, cmdPlateH);
    presetPlateTexture = generatePlateTexture(presetPlateW, presetPlateH);
    
}

RotateMeAudioProcessorEditor::~RotateMeAudioProcessorEditor()
{
    dryWetAttachment.reset();
    satAttachment.reset();
    speedAttachment.reset();
    brakeAttachment.reset();
    
    this->setLookAndFeel(nullptr);
    
}

//==============================================================================
void RotateMeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage(woodTexture, getLocalBounds().toFloat());
    
    
    Rectangle<float> commandPlateBounds(-78.0f + padding_left, padding_top -90.0f, (float)cmdPlateW, (float)cmdPlateH);
    Rectangle<float> presetPlateBounds(presetPlateX, presetPlateY, (float)presetPlateW, (float)presetPlateH);
    g.drawImage(commandPlateTexture, commandPlateBounds);
    g.setColour(juce::Colour::fromRGB(69, 50, 32));
    g.drawRoundedRectangle(commandPlateBounds, plateCornerRadius, plateBorderThickness);
    g.drawImage(presetPlateTexture, presetPlateBounds);
    g.setColour(juce::Colour::fromRGB(69, 50, 32));
    g.drawRoundedRectangle(presetPlateBounds, plateCornerRadius, plateBorderThickness);

    
    auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Jauza_otf,
        BinaryData::Jauza_otfSize
    );
    juce::FontOptions opts = FontOptions(typeface).withHeight(48.f).withStyle("plain");
    juce::Font jauzaFont(opts);
    
    
    g.setColour(juce::Colours::lightgrey);
    g.setFont(jauzaFont);
    g.drawText("RotateMe", cmdPlateX + cmdPlateW, padding_top - 340, 600, 600, juce::Justification::centred);
    g.setFont(juce::FontOptions(15.0f));
    g.drawText("Speed", cmdPlateX + 20, cmdPlateY + 145, textBoxW, textBoxH, juce::Justification::centred);
    g.drawText("Saturation Type", cmdPlateX - 260 + cmdPlateW, cmdPlateY - 35, textBoxW + 26, textBoxH, juce::Justification::centred);
    g.drawText("Drive", cmdPlateX + 20, cmdPlateY - 35, textBoxW, textBoxH, juce::Justification::centred);
    g.drawText("Brake", cmdPlateX - 260 + cmdPlateW, cmdPlateY + 145, textBoxW, textBoxH, juce::Justification::centred);
    g.drawText("Dry/Wet", cmdPlateX + 120, cmdPlateY + 300, textBoxW, textBoxH, juce::Justification::centred);
    
    
    Rectangle<float> upperHole(upperHoleX, upperHoleY, holeW, holeH);
    Rectangle<float> bottomHole(bottomHoleX, bottomHoleY, holeW, holeH);
    
    
    float holeCornerSize = 18.0f;
    g.setGradientFill(ColourGradient(
                                     Colour::fromRGB(40, 30, 20),
                                     0, 0,
                                     Colour::fromRGB(40, 30, 20),
                                     holeW, 0,
                                     true
                                     ));
    g.fillRoundedRectangle(upperHole, holeCornerSize);
    g.fillRoundedRectangle(bottomHole, holeCornerSize);
    g.setColour(juce::Colour::fromRGB(69, 50, 32));
    g.drawRoundedRectangle(upperHole, plateCornerRadius, plateBorderThickness);
    g.drawRoundedRectangle(bottomHole, plateCornerRadius, plateBorderThickness);
    
}
void RotateMeAudioProcessorEditor::resized()
{
    woodTexture = Image(Image::RGB, getWidth(), getHeight(), false);
    
    generateWoodTexture();
    
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(nextPreset);
    addAndMakeVisible(previousPreset);
    addAndMakeVisible(load);
    addAndMakeVisible(save);
    
    
    presetBrowser.setBounds(presetPlateX + 40 + presetButtonW, presetPlateY + 20, presetLabelW, presetLabelH);
    previousPreset.setBounds(presetPlateX + 20, presetPlateY + 20, presetButtonW, presetButtonH);
    nextPreset.setBounds(presetPlateX + presetLabelW + 60 + presetButtonW, presetPlateY + 20, presetButtonW, presetButtonH);
    load.setBounds(presetPlateX + 20, presetPlateY + 100, presetButtonW, presetButtonH);
    save.setBounds(presetPlateX + presetLabelW + 60 + presetButtonW, presetPlateY + 100, presetButtonW, presetButtonH);
}


void RotateMeAudioProcessorEditor::setupSlider(Slider &slider, int x, int y, int w, int h, float rotationWindow)
{
    slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    slider.setRotaryParameters(rotationWindow * -MathConstants<float>::pi, rotationWindow * MathConstants<float>::pi, true);
    addAndMakeVisible(&slider);
    slider.setBounds(x, y, w, h);
}

Image RotateMeAudioProcessorEditor::generateWoodTexture()
{
    auto r = getLocalBounds().toFloat();
    Image texture = juce::Image(juce::Image::RGB, getWidth(), getHeight(), false);
    juce::Graphics g(texture);
    
    
    g.setGradientFill(juce::ColourGradient(
        juce::Colour::fromRGB(200, 150, 80),
        0, 0,
        juce::Colour::fromRGB(230, 190, 130),
        0, (float)getHeight(),
        false
    ));
    g.fillRect(r);
    
    
    juce::Random rand(12345);
    for (int x = 0; x < getHeight(); ++x)
    {
        float noise = (rand.nextFloat() - 0.5f) * 8.0f;
        float grain = std::sin((x + noise) * 1.25f);
        grain = (grain + 1.0f) * 0.5f;
        juce::Colour c = juce::Colour::fromRGB(
            (uint8)(60 + grain * 40),
            (uint8)(35 + grain * 25),
            (uint8)(20 + grain * 15)
        );
        g.setColour(c);
        g.drawLine(0.0f, (float)x, (float)getWidth(), (float)x);
    }
    
    
    juce::ColourGradient gloss(
        juce::Colours::white.withAlpha(0.08f),
        r.getCentreX(), r.getY() + 40,
        juce::Colours::transparentBlack,
        r.getCentreX(), r.getBottom(),
        true
    );
    g.setGradientFill(gloss);
    g.fillRect(r);
    
    
    g.setGradientFill(juce::ColourGradient(
        juce::Colours::transparentBlack,
        r.getCentreX(), r.getCentreY(),
        juce::Colours::black.withAlpha(0.3f),
        r.getX(), r.getY(),
        true
    ));
    g.fillRect(r);
    return texture;
}


Image RotateMeAudioProcessorEditor::generatePlateTexture(int width, int height)
{
    Image texture = juce::Image(juce::Image::RGB, width, height, false);
    Graphics g(texture);
    
    
    g.setGradientFill(juce::ColourGradient(
        juce::Colour::fromRGB(180, 180, 185),
        0, 0,
        juce::Colour::fromRGB(110, 110, 115),
        0, (float)getWidth(),
        false
    ));
    g.fillRoundedRectangle(0, 0, width, height, 12.0f);

    
    juce::Random rand(123456);
    for (int x = 0; x < width; ++x)
    {
        float noise = (rand.nextFloat() - 0.5f) * 0.15f;
        float brightness = 0.5f + noise;
        juce::Colour c = juce::Colour::fromFloatRGBA(
            brightness, brightness, brightness, 0.12f
        );
        g.setColour(c);
        g.drawLine((float)x, 0.0f, (float)x, (float)height);
    }


    g.setGradientFill(juce::ColourGradient(
        juce::Colours::white.withAlpha(0.10f),
        width * 0.5f, height * 0.15f,
        juce::Colours::transparentWhite,
        width * 0.5f, height * 0.85f,
        false
    ));
    g.fillRoundedRectangle(0, 0, width, height, 12.0f);


    g.setGradientFill(juce::ColourGradient(
        juce::Colours::transparentBlack,
        width * 0.5f, height * 0.5f,
        juce::Colours::black.withAlpha(0.25f),
        0, 0,
        true
    ));
    g.fillRoundedRectangle(0, 0, width, height, 12.0f);
    return texture;
}


void RotateMeAudioProcessorEditor::updatePresetBrowser()
{
    presetBrowser.setText(audioProcessor.getCurrentPresetName(), dontSendNotification);
}


void RotateMeAudioProcessorEditor::loadPreset()
{
    auto defaultLocation = File::getSpecialLocation(File::commonDocumentsDirectory);
    FileChooser chooser("Select preset...", defaultLocation, "*.xml");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        MemoryBlock data;
        
        if (file.loadFileAsData(data))
        {
            audioProcessor.setStateInformation(data.getData(), (int)data.getSize());
        }
    }
}


void RotateMeAudioProcessorEditor::savePreset()
{
    auto defaultLocation = File::getSpecialLocation(File::commonDocumentsDirectory);
    FileChooser chooser("Save preset...", defaultLocation, "*.xml");
    
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult();
        if (file.existsAsFile())
        {
            file.deleteFile();
        }
        
        FileOutputStream output(file);
        if (output.openedOk())
        {
            MemoryBlock data;
            audioProcessor.getStateInformation(data);
            output.write(data.getData(), data.getSize());
        }
    }
}


