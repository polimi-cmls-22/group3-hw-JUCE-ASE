#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

namespace ParametersGroupIDs
{
    const static juce::String envelopeGroup{ "envelope" };
    const static juce::String oscillatorsGroup{ "oscillators" };
    const static juce::String lfoGroup{ "lfo" };
    const static juce::String noiseGroup{ "noise" };
    const static juce::String hpfilterGroup{ "HPfilter" };
    const static juce::String lpfilterGroup{ "LPfilter" };
    const static juce::String peakfilterGroup{ "peakfilter" };
    const static juce::String outputGroup{ "output" };
}

namespace ParametersIDs
{
    const static juce::String paramAttack{ "ATTACK" };
    const static juce::String paramDecay{ "DECAY" };
    const static juce::String paramSustain{ "SUSTAIN" };
    const static juce::String paramRelease{ "RELEASE" };
    const static juce::String paramOscWaveType{ "OSCWAVETYPE" };
    const static juce::String paramOscGain{ "OSCGAIN" };
    const static juce::String paramNoiseGain{ "NOISEGAIN" };
    const static juce::String paramOutputGain{ "OUTPUTGAIN" };
    const static juce::String paramLFOAmount{ "PARAMLFOGAIN" };
    const static juce::String paramLFOFrequency{ "PARAMLFOWAVEFREQUENCY" };
    const static juce::String paramHPFilterCutOffFreq{ "HPFILTERCUTOFF" };
    const static juce::String paramHPFilterResonance{ "HPFILTERRESONANCE" };
    const static juce::String paramLPFilterCutOffFreq{ "LPFILTERCUTOFF" };
    const static juce::String paramLPFilterResonance{ "LPFILTERRESONANCE" };
    const static juce::String paramPeakFilterCutOffFreq{ "PEAKFILTERCUTOFF" };
    const static juce::String paramPeakFilterResonance{ "PEAKFILTERRESONANCE" }; 
    const static juce::String paramPeakFilterGain{ "PEAKFILTERGAIN" };
    const static juce::String paramRenderBlockBufferSize{ "PARAMRENDERBLOCKBUFFERSIZE" };
}

class ParametersManager  {

public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    static juce::Array<juce::AudioProcessorParameter*> getParametersForGroup(juce::AudioProcessorValueTreeState& state,
                                                                              const juce::String& groupId);

private:
    static void addEnvelopeParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addOscillatorsParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addLFOParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addNoiseParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addHPFiltersParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addLPFiltersParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addPeakFiltersParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    static void addOutputParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout);
};
