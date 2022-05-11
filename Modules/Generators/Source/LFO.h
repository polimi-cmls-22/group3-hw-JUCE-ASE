#pragma once

namespace Generators {

    class LFO
    {
    public:
        LFO() = default;

        void prepare(juce::dsp::ProcessSpec &spec);
        void process(juce::AudioBuffer<float>& buffer,  int startSample, int numSamples);
        void updateParameters(float frequency, float amount);

    private:
        juce::dsp::Oscillator<float> lfo;
        float amount;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFO)
    };

}
