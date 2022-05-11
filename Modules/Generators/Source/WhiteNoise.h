#pragma once

namespace Generators {

    class WhiteNoise
    {
    public:
        WhiteNoise() = default;

        void prepare(juce::dsp::ProcessSpec &spec);
        void process(juce::AudioBuffer<float>& buffer,  int startSample, int numSamples);
        void updateParameters(float gain);

    private:
        juce::Random randomProcess;
        float scale = 2.0f;
        float gain { 0.1f };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WhiteNoise)
    };

}
