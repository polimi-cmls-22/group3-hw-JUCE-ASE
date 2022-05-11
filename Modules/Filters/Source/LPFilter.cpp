#include "LPFilter.h"

namespace Filters {

    void LPFilter::prepare(juce::dsp::ProcessSpec& spec)
    {
        this->filter.reset();
        this->filter.prepare(spec);
    }

    void LPFilter::process(juce::dsp::ProcessContextReplacing<float> context)
    {
        this->filter.process(context);
    }

    void LPFilter::updateParameters(int sampleRate, float cutoff, float resonance)
    {
        *this->filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, resonance);
    }

}