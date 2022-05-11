#include "LFO.h"

namespace Generators {

    void LFO::prepare(juce::dsp::ProcessSpec &spec)
    {
        this->lfo.prepare(spec);
    }

    void LFO::process(juce::AudioBuffer<float>& buffer,  int startSample, int numSamples)
    {
        juce::ignoreUnused(startSample, numSamples);
        auto lfoModulation = juce::jmap(lfo.processSample(0.0f), -1.0f, 1.0f, 1.0f - this->amount, 1.0f + this->amount);
        buffer.applyGain(lfoModulation);
    }

    void LFO::updateParameters(float frequency, float amount)
    {
        this->lfo.initialise([](float x) { return std::sin(x); });
        this->lfo.setFrequency(frequency);
        this->amount = amount;
    }

}