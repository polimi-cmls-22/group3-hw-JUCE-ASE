#include "Envelope.h"

namespace Generators {

    Envelope::Envelope(): juce::ADSR() {}

    void Envelope::prepare(juce::dsp::ProcessSpec &spec)
    {
        this->reset();
        this->setSampleRate(spec.sampleRate);
    }

    void Envelope::process(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
    {
        this->applyEnvelopeToBuffer(buffer, startSample, numSamples);
    }

    void Envelope::updateParameters(float attack, float decay, float sustain, float release)
    {
        parameters.attack = attack;
        parameters.decay = decay;
        parameters.sustain = sustain;
        parameters.release = release;
        setParameters(parameters);
    }

}