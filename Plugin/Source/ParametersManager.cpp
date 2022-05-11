#include "ParametersManager.h"
#include "OranJam.h"


//==================================================================================================
juce::AudioProcessorValueTreeState::ParameterLayout ParametersManager::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    ParametersManager::addEnvelopeParameters(layout);
    ParametersManager::addOscillatorsParameters(layout);
    ParametersManager::addLFOParameters(layout);
    ParametersManager::addNoiseParameters(layout);
    ParametersManager::addOutputParameters(layout);
    ParametersManager::addHPFiltersParameters(layout);
    ParametersManager::addLPFiltersParameters(layout);
    ParametersManager::addPeakFiltersParameters(layout);
    return layout;
}

juce::Array<juce::AudioProcessorParameter*> ParametersManager::getParametersForGroup(juce::AudioProcessorValueTreeState& state,
                                                                                     const juce::String& groupId)
{
    auto* group = dynamic_cast<juce::AudioProcessorParameterGroup*>(state.getParameter(groupId));
    jassert(group);
    return group->getParameters(false);
}



//==================================================================================================
void ParametersManager::addEnvelopeParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::envelopeGroup, "Envelope", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramAttack, "Attack",
                                                                juce::NormalisableRange<float> (0.001f, 5.0f, 0.01f, 0.3f), 0.1f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramDecay, "Decay",
                                                                juce::NormalisableRange<float> (0.001f, 5.0f, 0.01f, 0.3f), 0.1f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramSustain, "Sustain",
                                                                juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramRelease, "Release",
                                                                juce::NormalisableRange<float> (0.001f, 5.0f, 0.01f, 0.3f), 0.1f));
    layout.add(std::move(group));
}

void ParametersManager::addOscillatorsParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::oscillatorsGroup, "Oscillators", "|");
    for (int i = 0; i < OranJam::numOscillators; ++i)
    {
        group->addChild(std::make_unique<juce::AudioParameterChoice>(ParametersIDs::paramOscWaveType + juce::String(i),
                                                                     "Osc" + juce::String(i) + "Wave Type",
                                                                     juce::StringArray {"Sine", "Saw", "Square"}, 0));
        group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramOscGain + juce::String(i),
                                                                    "Osc" + juce::String(i) + "Gain",
                                                                    juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    }
    layout.add(std::move(group));
}

void ParametersManager::addLFOParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::lfoGroup, "LFO", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramLFOAmount, "LFO Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramLFOFrequency, "LFO Frequency",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 1.0f));
    layout.add(std::move(group));
}

void ParametersManager::addNoiseParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::noiseGroup, "Noise", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramNoiseGain, "Noise Gain",
                                                                juce::NormalisableRange<float> { 0.0f, 1.0f, }, 0.0f));
    layout.add(std::move(group));
}

void ParametersManager::addHPFiltersParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::hpfilterGroup, "HPFilter", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramHPFilterCutOffFreq, "HPF CutOff",
        juce::NormalisableRange<float>(20.0f, 22000.0f, 0.01f, 0.25f), 800.0f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramHPFilterResonance, "HPF Resonance",
        juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f, 0.5f), 1.0));
    layout.add(std::move(group));
}

void ParametersManager::addLPFiltersParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::lpfilterGroup, "LPFilter", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramLPFilterCutOffFreq, "LPF CutOff",
        juce::NormalisableRange<float>(20.0f, 22000.0f, 0.01f, 0.25f), 800.0f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramLPFilterResonance, "LPF Resonance",
        juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f, 0.5f), 1.0));
    layout.add(std::move(group));
}

void ParametersManager::addPeakFiltersParameters(juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::peakfilterGroup, "PeakFilter", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramPeakFilterCutOffFreq, "PeakF CutOff",
                                                                juce::NormalisableRange<float>(20.0f, 20000.0f, 0.01f, 0.25f), 800.0f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramPeakFilterResonance, "PeakF Resonance",
                                                                juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f, 0.5f), 1.0));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramPeakFilterGain, "PeakF Gain",
                                                                juce::NormalisableRange<float>(0.1f, 5.0f, 0.1f), 1.0));
    layout.add(std::move(group));
}

void ParametersManager::addOutputParameters (juce::AudioProcessorValueTreeState::ParameterLayout& layout)
{
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>(ParametersGroupIDs::outputGroup, "Output", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(ParametersIDs::paramOutputGain, "Master Gain",
                                                            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.70f));
    layout.add(std::move(group));
}

