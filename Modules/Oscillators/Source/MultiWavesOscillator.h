#pragma once

#include "juce_dsp/juce_dsp.h"
#include "juce_audio_processors/juce_audio_processors.h"

namespace Oscillators
{
    class MultiWavesOscillator
    {
    public:
        MultiWavesOscillator();

        void prepare(juce::dsp::ProcessSpec& spec);
        void process(juce::dsp::ProcessContextReplacing<float> context);
        void reset();
        void updateParameters(int waveType, int midiNoteNumber, float gain);

        float getLevel();
        void setLevel(float value);
        int getWaveType();
        void setWaveType(int waveType);
        float getWaveFrequency();
        void setWaveFrequency(int midiNoteNumber);

    private:
        int currentWaveType;
        enum { oscillatorIndex, gainIndex };
        juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> processorChain;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiWavesOscillator)
    };
}
