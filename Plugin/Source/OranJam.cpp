#include "OranJam.h"
#include "ParametersManager.h"

//==============================================================================
// SubtractiveSynth implementation
//==============================================================================
void OranJam::prepareToPlay(double sampleRate, int samplesPerBlock, int totalNumOutputChannels)
{
    this->setCurrentPlaybackSampleRate(sampleRate);
    this->prepareVoices(sampleRate, samplesPerBlock, totalNumOutputChannels);
}

void OranJam::prepareVoices(double sampleRate, int samplesPerBlock, int totalNumOutputChannels)
{
    for (int i = 0; i < this->getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(this->getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, totalNumOutputChannels);
        }
    }
}

void OranJam::updateVoices(OranJam::SynthSound* sound)
{
    for (int i = 0; i < this->getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(this->getVoice(i)))
        {
            voice->update(sound);
        }
    }
}

//==============================================================================
// SynthSound implementation
//==============================================================================
OranJam::SynthSound::SynthSound(juce::AudioProcessorValueTreeState& state): juce::SynthesiserSound(),
                                                                            treeState(state) { }

//==============================================================================
// SynthVoice implementation
//==============================================================================
OranJam::SynthVoice::SynthVoice(): juce::SynthesiserVoice()
{
    for (int i = 0; i < OranJam::numOscillators; ++i)
    {
        oscillators.push_back(std::make_unique<Oscillators::MultiWavesOscillator>());
    }

    oscillatorBuffer.setSize(1, internalBufferSize);
    voiceBuffer.setSize(1, internalBufferSize);
}

bool OranJam::SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void OranJam::SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound,
                                    int currentPitchWheelPosition)
{
    juce::ignoreUnused(velocity, currentPitchWheelPosition);

    if (this->canPlaySound(sound)) {
        // Update the voice with the current parameters
        this->update(dynamic_cast<SynthSound *>(sound), midiNoteNumber);
        soundEnvelope.noteOn();
    }
}

void OranJam::SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);

    soundEnvelope.noteOff();

    if (!allowTailOff || !soundEnvelope.isActive())
        this->clearCurrentNote();
}

void OranJam::SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    juce::ignoreUnused(controllerNumber, newControllerValue);
}

void OranJam::SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    juce::ignoreUnused(newPitchWheelValue);
}

void OranJam::SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec{static_cast<double>(sampleRate),
                                static_cast<juce::uint32>(internalBufferSize),
                                static_cast<juce::uint32>(outputChannels)};

    for (auto& oscillator : oscillators) {
        oscillator->prepare(spec);
    }
    whiteNoise.prepare(spec);
    amplitudeLfo.prepare(spec);
    soundEnvelope.prepare(spec);
    hpFilter.prepare(spec);
    lpFilter.prepare(spec);
    peakFilter.prepare(spec);
    smoothedGain.reset(sampleRate, 0.01f);
}

void OranJam::SynthVoice::update(OranJam::SynthSound* sound, int midiNoteNumber)
{
    auto& currentState = sound->getCurrentState();

    // Update oscillators parameters
    for (unsigned long i = 0; i < OranJam::numOscillators; ++i)
    {
        auto& oscWaveType = *currentState.getRawParameterValue(ParametersIDs::paramOscWaveType + juce::String(i));
        auto& oscGain = *currentState.getRawParameterValue(ParametersIDs::paramOscGain + juce::String(i));
        oscillators[i]->updateParameters(oscWaveType.load(), midiNoteNumber, oscGain.load());
    }

    // Update sound envelope
    auto& attack = *currentState.getRawParameterValue(ParametersIDs::paramAttack);
    auto& decay = *currentState.getRawParameterValue(ParametersIDs::paramDecay);
    auto& sustain = *currentState.getRawParameterValue(ParametersIDs::paramSustain);
    auto& release = *currentState.getRawParameterValue(ParametersIDs::paramRelease);
    soundEnvelope.updateParameters(attack.load(), decay.load(), sustain.load(), release.load());

    // Update noise
    auto& noiseGain = *currentState.getRawParameterValue(ParametersIDs::paramNoiseGain);
    whiteNoise.updateParameters(noiseGain);

    // Update LFO
    auto& LFOFrequency = *currentState.getRawParameterValue(ParametersIDs::paramLFOFrequency);
    auto& LFOAmount = *currentState.getRawParameterValue(ParametersIDs::paramLFOAmount);
    amplitudeLfo.updateParameters(LFOFrequency, LFOAmount);

    // Update filters
    // HP
    auto& hpCutoff = *currentState.getRawParameterValue(ParametersIDs::paramHPFilterCutOffFreq);
    auto& hpResonance = *currentState.getRawParameterValue(ParametersIDs::paramHPFilterResonance);
    hpFilter.updateParameters(this->getSampleRate(), hpCutoff, hpResonance);
    // LP
    auto& lpCutoff = *currentState.getRawParameterValue(ParametersIDs::paramLPFilterCutOffFreq);
    auto& lpResonance = *currentState.getRawParameterValue(ParametersIDs::paramLPFilterResonance);
    lpFilter.updateParameters(this->getSampleRate(), lpCutoff, lpResonance);
    // Peak
    auto& peakCutoff = *currentState.getRawParameterValue(ParametersIDs::paramPeakFilterCutOffFreq);
    auto& peakResonance = *currentState.getRawParameterValue(ParametersIDs::paramPeakFilterResonance);
    auto& peakGain = *currentState.getRawParameterValue(ParametersIDs::paramPeakFilterGain);
    peakFilter.updateParameters(this->getSampleRate(), peakCutoff, peakResonance, peakGain);

    // Update master gain
    auto& masterGainLevel = *currentState.getRawParameterValue(ParametersIDs::paramOutputGain);
    smoothedGain.setTargetValue(masterGainLevel.load());
}

void OranJam::SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample,
                                          int numSamples)
{
    // If the voice is silent do nothing
    if (!this->isVoiceActive())
        return;

    while (numSamples > 0)
    {
        auto samplesChunk = std::min(numSamples, oscillatorBuffer.getNumSamples());
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            // Clear the voice buffer before processing each channel
            voiceBuffer.clear();

            // Get an audio block to feed the oscillator's processor chain
            auto oscAudioBlock = juce::dsp::AudioBlock<float> (oscillatorBuffer).getSingleChannelBlock(0).getSubBlock(0, static_cast<size_t>((samplesChunk)));
            juce::dsp::ProcessContextReplacing<float> oscProcessContext (oscAudioBlock);

            // For each oscillator process the sample in the oscillator's buffer and add them to the voice buffer
            for (auto& oscillator : oscillators)
            {
                auto oscGain = oscillator->getLevel();
                if (oscGain < 0.01) continue;
                oscillatorBuffer.clear();
                oscillator->process(oscProcessContext);
                voiceBuffer.addFrom(0, 0, oscillatorBuffer.getReadPointer(0), samplesChunk);
            }

            // Apply white noise
            whiteNoise.process(voiceBuffer, 0, samplesChunk);

            // Apply gain LFO
            amplitudeLfo.process(voiceBuffer, 0, samplesChunk);

            // Apply envelope
            soundEnvelope.process(voiceBuffer, 0, samplesChunk);

            // Apply filters
            auto voiceAudioBlock = juce::dsp::AudioBlock<float> (voiceBuffer).getSingleChannelBlock(0).getSubBlock(0, static_cast<size_t>((samplesChunk)));
            juce::dsp::ProcessContextReplacing<float> voiceProcessContext (voiceAudioBlock);
            hpFilter.process(voiceProcessContext);
            lpFilter.process(voiceProcessContext);
            peakFilter.process(voiceProcessContext);

            // Output gain
            smoothedGain.applyGain(voiceBuffer, samplesChunk);

            // Add the voice buffer to the output buffer
            outputBuffer.addFrom(channel, startSample, voiceBuffer.getReadPointer(0), samplesChunk);
        }

        startSample += samplesChunk;
        numSamples -= samplesChunk;

        // Send a message to the synthesizer if the played sound note finishes during the rendering
        if (!soundEnvelope.isActive())
            this->clearCurrentNote();
    }

}