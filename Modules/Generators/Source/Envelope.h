#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_dsp/juce_dsp.h"

namespace Generators {

    class Envelope : public juce::ADSR
    {
    public:
        Envelope();

        void prepare(juce::dsp::ProcessSpec &spec);
        void process(juce::AudioBuffer<float>& buffer,  int startSample, int numSamples);
        void updateParameters(float attack, float decay, float sustain, float release);

    private:
        juce::ADSR::Parameters parameters;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
    };

}
