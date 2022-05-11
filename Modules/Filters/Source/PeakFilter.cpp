#include "PeakFilter.h"

namespace Filters {

    void PeakFilter::prepare(juce::dsp::ProcessSpec& spec)
    {
        this->filter.reset();
        this->filter.prepare(spec);
    }

    void PeakFilter::process(juce::dsp::ProcessContextReplacing<float> context)
    {
        this->filter.process(context);
    }

    void PeakFilter::updateParameters(int sampleRate, float cutoff, float resonance, float gain)
    {
        *this->filter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, cutoff, resonance, gain);
    }

}