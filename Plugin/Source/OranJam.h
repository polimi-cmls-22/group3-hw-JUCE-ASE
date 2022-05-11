#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include "foleys_gui_magic/foleys_gui_magic.h"
#include "Oscillators/Oscillators.h"
#include "Generators/Generators.h"
#include "Filters/Filters.h"

class OranJam : public juce::Synthesiser
{
public:
    static constexpr int numOscillators { 3 };
    static constexpr int numVoices { 4 };

    OranJam() = default;

    class SynthSound : public juce::SynthesiserSound
    {
    public:
        explicit SynthSound(juce::AudioProcessorValueTreeState& state);

        bool appliesToNote (int) override { return true; }
        bool appliesToChannel (int) override { return true; }

        juce::AudioProcessorValueTreeState& getCurrentState() { return treeState; }

    private:
        juce::AudioProcessorValueTreeState& treeState;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSound)
    };

    class SynthVoice : public juce::SynthesiserVoice
    {
    public:
        explicit SynthVoice();

        bool canPlaySound (juce::SynthesiserSound *) override;
        void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
        void stopNote (float velocity, bool allowTailOff) override;
        void controllerMoved (int controllerNumber, int newControllerValue) override;
        void pitchWheelMoved (int newPitchWheelValue) override;
        void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

        void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
        void update(SynthSound* sound, int midiNoteNumber = 0);

    private:
        int internalBufferSize =  1;
        juce::AudioBuffer<float> voiceBuffer;
        juce::AudioBuffer<float> oscillatorBuffer;
        // Processors
        std::vector<std::unique_ptr<Oscillators::MultiWavesOscillator>> oscillators;
        Generators::WhiteNoise whiteNoise;
        Generators::Envelope soundEnvelope;
        Generators::LFO amplitudeLfo;
        Filters::HPFilter hpFilter;
        Filters::LPFilter lpFilter;
        Filters::PeakFilter peakFilter;
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
    };

    void prepareToPlay(double sampleRate, int samplesPerBlock, int totalNumOutputChannels);
    void updateVoices(SynthSound* sound);

private:
    void prepareVoices(double sampleRate, int samplesPerBlock, int totalNumOutputChannels);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OranJam)
};