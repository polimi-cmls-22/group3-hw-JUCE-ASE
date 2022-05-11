#include "HPFilter.h"

namespace Filters {

    void HPFilter::prepare(juce::dsp::ProcessSpec &spec)
    {
        this->filter.reset();
        this->filter.prepare(spec);
    }

    void HPFilter::process(juce::dsp::ProcessContextReplacing<float> context)
    {
        this->filter.process(context);
    }

    void HPFilter::updateParameters(int sampleRate, float cutoff, float resonance)
    {
        *this->filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutoff, resonance);
    }

}

