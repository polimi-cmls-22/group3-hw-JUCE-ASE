#include "WhiteNoise.h"

namespace Generators {

    void WhiteNoise::prepare(juce::dsp::ProcessSpec &spec)
    {
        juce::ignoreUnused(spec);
    }

    void WhiteNoise::process(juce::AudioBuffer<float>& buffer,  int startSample, int numSamples)
    {
        auto levelScale = this->gain * this->scale;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* pointer = buffer.getWritePointer(channel, 0);
            for (int sample = startSample; sample < numSamples; ++sample)
            {
                float noise = randomProcess.nextFloat() * levelScale - this->gain;
                pointer[sample] += noise;
            }
        }
    }

    void WhiteNoise::updateParameters(float gain)
    {
        this->gain = gain;
    }

}