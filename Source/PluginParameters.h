#pragma once

#include <JuceHeader.h>

namespace Parameters
{
    // === PARAMETER IDs ===
    static const String nameDryWet     = "DW";
    static const String nameModSpeed   = "MS";
    static const String nameSatAmount  = "SA";
    static const String nameBrake      = "BK";

    // === DEFAULT VALUES ===
    static const float defaultDryWet     = 0.5f;
    static const float defaultSatAmount  = 1.0f;
    static const int   defaultModSpeed   = 0;     // 0 = Slow, 1 = Fast
    static const bool  defaultBraking    = false;

    // === DSP DEFAULTS (non parametrici) ===
    static const float defaultPitchTime  = 0.006f;
    static const float defaultAmpValue   = 0.3f;

    // ===========================================
    // PARAMETER LAYOUT
    // ===========================================
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        // ID univoci: 1,2,3,4
        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID(nameDryWet, 1),
            "Dry/Wet",
            NormalisableRange<float>(0.0f, 1.0f, 0.001f),
            defaultDryWet));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParameterID(nameSatAmount, 2),
            "Drive",
            NormalisableRange<float>(0.0f, 12.0f, 0.01f, 0.5f),
            defaultSatAmount));

        // Slow/Fast → Choice
        params.push_back(std::make_unique<AudioParameterChoice>(
            ParameterID(nameModSpeed, 3),
            "Speed",
            StringArray { "Slow", "Fast" },
            defaultModSpeed));

        params.push_back(std::make_unique<AudioParameterBool>(
            ParameterID(nameBrake, 4),
            "Brake",
            defaultBraking));

        return { params.begin(), params.end() };
    }

    // ===========================================
    // LISTENER
    // ===========================================
    static void addGlobalListener(AudioProcessorValueTreeState& vts,
                                  AudioProcessorValueTreeState::Listener* listener)
    {
        std::unique_ptr<XmlElement> xml(vts.copyState().createXml());

        for (auto* element : xml->getChildWithTagNameIterator("PARAM"))
        {
            const String& id = element->getStringAttribute("id");
            vts.addParameterListener(id, listener);
        }
    }
}
