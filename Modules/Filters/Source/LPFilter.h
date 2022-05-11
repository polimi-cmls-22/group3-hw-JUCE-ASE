#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_dsp/juce_dsp.h"

namespace Filters {

    class LPFilter
    {
    public:
        LPFilter() = default;

        void prepare(juce::dsp::ProcessSpec& spec);
        void process(juce::dsp::ProcessContextReplacing<float> context);
        void updateParameters(int sampleRate, float cutoff, float resonance);

    private:
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LPFilter)
    };

}