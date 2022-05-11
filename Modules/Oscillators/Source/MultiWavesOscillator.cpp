#include "MultiWavesOscillator.h"

namespace Oscillators {

    MultiWavesOscillator::MultiWavesOscillator() {
        this->currentWaveType = 0;
        this->setWaveType(currentWaveType);
    }

    void MultiWavesOscillator::prepare(juce::dsp::ProcessSpec &spec) {
        processorChain.prepare(spec);
    }

    void MultiWavesOscillator::process(juce::dsp::ProcessContextReplacing<float> context)
    {
        processorChain.process(context);
    }

    void MultiWavesOscillator::reset()
    {
        processorChain.reset();
    }

    void MultiWavesOscillator::updateParameters(int waveType, int midiNoteNumber, float gain)
    {
        this->setWaveType(waveType);
        this->setWaveFrequency(midiNoteNumber);
        this->setLevel(gain);
    }

    int MultiWavesOscillator::getWaveType()
    {
        return currentWaveType;
    }

    // TODO - Move wave generation math functions to an external module
    void MultiWavesOscillator::setWaveType(int choice) {
        auto& oscillator = this->processorChain.template get<oscillatorIndex>();
        switch (choice) {
            case 0:
                // Sine Wave
                oscillator.initialise([](float x) { return std::sin(x); });
                break;
            case 1:
                // Saw Tooth Wave
                oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi; });
                break;
            case 2:
                // Square Wave
                oscillator.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
                break;
            default:
                jassertfalse;
                break;
        }
        this->currentWaveType = choice;
    }

    float MultiWavesOscillator::getWaveFrequency()
    {
        auto& oscillator = this->processorChain.template get<oscillatorIndex>();
        return oscillator.getFrequency();
    }

    void MultiWavesOscillator::setWaveFrequency(int midiNoteNumber) {
        if (midiNoteNumber != 0) {
            auto& oscillator = this->processorChain.template get<oscillatorIndex>();
            oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        }
    }

    float MultiWavesOscillator::getLevel()
    {
        auto& gain = this->processorChain.template get<gainIndex>();
        return gain.getGainLinear();
    }

    void MultiWavesOscillator::setLevel(float value)
    {
        auto& gain = this->processorChain.template get<gainIndex>();
        gain.setGainLinear(value);
    }

}
