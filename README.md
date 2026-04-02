# RotateMe

**RotateMe** is a faithful emulation of the legendary **Leslie 147** rotary speaker cabinet. Designed to capture the warm, physical essence of the original vintage hardware, RotateMe provides a rich, swirling modulation effect that brings life to organs, guitars, and synthesizers.

Built with C++ and the JUCE framework.

## Features

- **Authentic Rotary Emulation:** Captures the unique frequency and amplitude modulation of a physical rotating speaker.
- **Two Operation Modes:**
  - **Chorus (Slow):** For subtle, lush movement.
  - **Tremolo (Fast):** For the classic high-speed Leslie "shimmer".
- **Pre-Saturation Stage:** Includes a dedicated drive section with two selectable saturation types:
  - **Tube:** Warm, asymmetrical vintage-style saturation.
  - **Hard:** More aggressive clipping for a modern edge.
- **Brake Feature:** A momentary or toggle-able stop feature that simulates the physical inertia of the rotors slowing down.
- **Dry/Wet Control:** Easily blend the processed signal with the original.
- **Preset Browser:** Includes factory presets to get you started and the ability to save your own settings.

## Installation

Download the pre-compiled binaries for your system and copy them to your plugin directory.

### Windows (VST3)
Copy `.vst3` files to:
`C:\Program Files\Common Files\VST3`

### macOS (AU & VST3)
- **Audio Unit (AU):** `/Library/Audio/Plug-Ins/Components`
- **VST3:** `/Library/Audio/Plug-Ins/VST3`

### Linux (VST3)
Copy `.vst3` files to:
`/usr/lib/vst3` or `~/.vst3`

## Technical Details

RotateMe is developed using **JUCE**. The DSP architecture consists of:
- A custom **PitchDelay** and **Amplifier** module to simulate the Doppler effect and amplitude modulation of the rotors.
- A dedicated **Saturation** class with waveshaping algorithms.
- **LFO-driven modulation** with smoothed parameter transitions to emulate the physical acceleration of the Leslie motors.

## Building from Source

To build RotateMe from source, you will need:
1. The **JUCE** framework.
2. An IDE (Visual Studio, Xcode, or CLion/Makefile for Linux).
3. Open `RotateMe.jucer` in the **Projucer**, select your exporter, and build.

## License

This project is open-source and released under the **MIT License**.

## Contribution

This is a pre-release version of my first audio plugin. Feedback, bug reports, and pull requests are welcome!

---
*Developed by **Giovanni Dalla Villa** in collaboration with **Laboratorio di Informatica Musicale (LIM)**, Università degli Studi di Milano.*
