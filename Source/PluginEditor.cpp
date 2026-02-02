#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"
#include "Layout.h"

using namespace MyColours;
using namespace GUI;



// RotateMeAudioProcessorEditor Class implementation
//==============================================================================
RotateMeAudioProcessorEditor::RotateMeAudioProcessorEditor (RotateMeAudioProcessor& p, AudioProcessorValueTreeState& valueTreeState)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(valueTreeState), commandPlate(commandPlateTexture), presetPlate(presetPlateTexture), rotaryVisualizer(p)
{
    woodTexture = generateWoodTexture();
    commandPlateTexture = generatePlateTexture(GUI::commandP.w, GUI::presetP.h);
    presetPlateTexture = generatePlateTexture(GUI::commandP.w, GUI::presetP.h);
    
    addAndMakeVisible(commandPlate);
    addAndMakeVisible(presetPlate);
    addAndMakeVisible(upperHole);
    addAndMakeVisible(bottomHole);
    addAndMakeVisible(credits);
    setupPresetBrowser();
    updatePresetBrowser();
    
    setSize (window.w, window.h);
    
    setupSliderRotary(satSlider, satLabel, commandP.x, commandP.y, cmdKnob.w, cmdKnob.h, cmdKnob.cornerSize + 0.5f, "Drive");
    setupSliderRotary(satTypeSlider, satTypeLabel, commandP.x + commandP.w - 280, commandP.y, cmdKnob.w, cmdKnob.h, cmdKnob.cornerSize, "Saturation Type");
    setupSliderRotary(speedSlider, speedLabel, commandP.x, commandP.y + 180, cmdKnob.w, cmdKnob.h, cmdKnob.cornerSize, "Speed");
    setupSliderRotary(brakeSlider, brakeLabel, commandP.x + commandP.w - 280, commandP.y + 180, cmdKnob.w, cmdKnob.h, cmdKnob.cornerSize, "Brake");
    setupSliderLinear(dryWetSlider, dwLabel, commandP.x - 15, commandP.y + 350, 350, 50, "Dry/Wet");
    
    dryWetAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameDryWet , dryWetSlider));
    satAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameSatAmount , satSlider));
    satTypeAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameSatType, satTypeSlider));
    speedAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameModSpeed , speedSlider));
    brakeAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameBrake , brakeSlider));
    
    addAndMakeVisible(rotaryVisualizer);
    
    this->setLookAndFeel(&myTheme);
}


RotateMeAudioProcessorEditor::~RotateMeAudioProcessorEditor()
{
    dryWetAttachment.reset();
    satAttachment.reset();
    satTypeAttachment.reset();
    speedAttachment.reset();
    brakeAttachment.reset();
    
    this->setLookAndFeel(nullptr);
}


//==============================================================================
void RotateMeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage(woodTexture, getLocalBounds().toFloat());
}


void RotateMeAudioProcessorEditor::resized()
{
    woodTexture = generateWoodTexture();
    
    commandPlate.setBounds(-78 + window.padding_left, window.padding_top - 90, commandP.w, commandP.h);
    presetPlate.setBounds(presetP.x, presetP.y, presetP.w, presetP.h);
    upperHole.setBounds(upperH.x, upperH.y, upperH.w, upperH.h);
    bottomHole.setBounds(lowerH.x, lowerH.y, lowerH.w, lowerH.h);
    
    credits.setBounds(getLocalBounds());
    presetBrowser.setBounds(presetP.x + 40 + presetB.w, presetP.y + 20, presetL.w, presetL.h);
    previousPreset.setBounds(presetP.x + 20, presetP.y + 20, presetB.w, presetB.h);
    nextPreset.setBounds(presetP.x + presetL.w + 60 + presetB.w, presetP.y+ 20, presetB.w, presetB.w);
    load.setBounds(presetP.x + 20, presetP.y+ 100, presetB.w, presetB.w);
    save.setBounds(presetP.x + presetL.w + 60 + presetB.w, presetP.y + 100, presetB.w, presetB.h);
    rotaryVisualizer.setBounds (visualizer.x,
                                visualizer.y,
                                visualizer.w,
                                visualizer.h);
}


void RotateMeAudioProcessorEditor::setupSliderRotary(Slider &slider, Label &label, int x, int y, int w, int h, float rotationWindow, String name)
{
    label.setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setText(name, dontSendNotification);
    label.setJustificationType(Justification::centred);
    label.attachToComponent(&slider, false);
    slider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    slider.setRotaryParameters((1.5 - rotationWindow) * MathConstants<float>::pi, (1.5 + rotationWindow) * MathConstants<float>::pi, true);
    addAndMakeVisible(&slider);
    addAndMakeVisible(label);
    slider.setBounds(x, y, w, h);
}


void RotateMeAudioProcessorEditor::setupSliderLinear(Slider &slider, Label &label, int x, int y, int w, int h, String name)
{
    label.setText(name, dontSendNotification);
    label.setJustificationType(Justification::centred);
    label.attachToComponent(&slider, false);
    slider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    slider.setBounds(x, y, w, h);
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
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
            (uint8)(120 + grain * 40),
            (uint8)(70 + grain * 25),
            (uint8)(60 + grain * 15)
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


void RotateMeAudioProcessorEditor::setupPresetBrowser()
{
    presetBrowser.setColour(Label::backgroundColourId, Colour::fromRGB(40, 40, 40));
    presetBrowser.setJustificationType(Justification::centred);
    presetBrowser.setText(audioProcessor.getCurrentPresetName(), dontSendNotification);
    
    nextPreset.onClick = [this]
    {
        audioProcessor.loadFactoryPreset(audioProcessor.currentPresetIndex - 1);
        currentPreset = audioProcessor.currentPresetIndex;
        updatePresetBrowser();
    };
    
    previousPreset.onClick = [this]
    {
        audioProcessor.loadFactoryPreset(audioProcessor.currentPresetIndex + 1);
        currentPreset = audioProcessor.currentPresetIndex;
        updatePresetBrowser();
    };
    
    load.onClick = [this] { loadPreset(); };
    save.onClick = [this] { savePreset(); };
    
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(nextPreset);
    addAndMakeVisible(previousPreset);
    addAndMakeVisible(load);
    addAndMakeVisible(save);
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
            audioProcessor.addPreset(file.getFileNameWithoutExtension(), data.getData(), (int)data.getSize());
        }
        updatePresetBrowser();
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


// RotaryVisualizer Class implementation
RotaryVisualizer::RotaryVisualizer(RotateMeAudioProcessor& p) : processor(p)
{
    hornOpening = visualizer.openingSize;
    neckWidth = visualizer.neckSize;
    hubSize = visualizer.hubSize;
    startTimerHz(60);
}


RotaryVisualizer::~RotaryVisualizer() {}


void RotaryVisualizer::setRotationSpeed(float newValue)
{
    speed = newValue;
}


void RotaryVisualizer::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat().reduced(10.0f);
    auto centre = area.getCentre();
    
    float hornLength = area.getWidth() * 0.40f;

    juce::Path horns;
    
    horns.startNewSubPath (centre.x, centre.y - neckWidth);
    horns.quadraticTo (centre.x + hornLength * 0.5f, centre.y - neckWidth,
                       centre.x + hornLength, centre.y - hornOpening);
    horns.lineTo (centre.x + hornLength, centre.y + hornOpening);
    horns.quadraticTo (centre.x + hornLength * 0.5f, centre.y + neckWidth,
                       centre.x, centre.y + neckWidth);
    
    horns.startNewSubPath (centre.x, centre.y - neckWidth);
    horns.quadraticTo (centre.x - hornLength * 0.5f, centre.y - neckWidth,
                       centre.x - hornLength, centre.y - hornOpening);
    horns.lineTo (centre.x - hornLength, centre.y + hornOpening);
    horns.quadraticTo (centre.x - hornLength * 0.5f, centre.y + neckWidth,
                       centre.x, centre.y + neckWidth);
    horns.closeSubPath();

    g.addTransform (juce::AffineTransform::rotation (currentAngle, centre.x, centre.y));

    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.fillPath (horns, juce::AffineTransform::translation (5.0f, 5.0f));

    juce::ColourGradient metal (hornMetalLight, 0, centre.y - hornOpening,
                                hornMetalDark, 0, centre.y + hornOpening, false);
    metal.addColour (0.5, juce::Colour::fromRGB (90, 93, 98));
    g.setGradientFill (metal);
    g.fillPath (horns);

    g.setColour (juce::Colour::fromRGB (5, 5, 7));
    juce::Rectangle<float> rightOpening (centre.x + hornLength - 4, centre.y - hornOpening, 8, hornOpening * 2);
    juce::Rectangle<float> leftOpening (centre.x - hornLength - 4, centre.y - hornOpening, 8, hornOpening * 2);
    g.fillEllipse (rightOpening);
    g.fillEllipse (leftOpening);

    juce::Path highlight;
    highlight.startNewSubPath (centre.x - hornLength + 10, centre.y - 2);
    highlight.lineTo (centre.x + hornLength - 10, centre.y - 2);
    g.setColour (juce::Colours::white.withAlpha (0.2f));
    g.strokePath (highlight, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.setColour (juce::Colours::white.withAlpha (0.3f));
    g.strokePath (horns, juce::PathStrokeType (1.0f));

    juce::Rectangle<float> hubRect (centre.x - hubSize/2, centre.y - hubSize/2, hubSize, hubSize);
    
    juce::ColourGradient hubGrad (hornHubLight, centre,
                                  hornHubDark, centre.translated (hubSize, hubSize), true);
    g.setGradientFill (hubGrad);
    g.fillEllipse (hubRect);
    
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawEllipse (hubRect, 1.5f);
}


void RotaryVisualizer::timerCallback()
{
    float speedFromDSP = processor.rotationSpeed.load();
    setRotationSpeed(speedFromDSP);
    
    auto speedToUse = (speed / 60.0f) * MathConstants<float>::twoPi / 60.0f;
    if (std::abs(speedToUse) > 0.0001f || std::abs(speedFromDSP) > 0.0001f)
    {
        currentAngle += speedToUse;
        if (currentAngle > MathConstants<float>::twoPi)
        {
            currentAngle -= MathConstants<float>::twoPi;
        }
    }
    repaint();
}
