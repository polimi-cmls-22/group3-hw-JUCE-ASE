#pragma once

#include "OranJam.h"
#include "foleys_gui_magic/foleys_gui_magic.h"

class SubtractiveSynthAudioProcessor : public foleys::MagicProcessor
{
public:
    SubtractiveSynthAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
    #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    double getTailLengthSeconds() const override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    OranJam synthesiser;

    juce::AudioProcessorValueTreeState treeState;
    foleys::MagicProcessorState magicState{ *this };

    // Magic GUI Visualization
    foleys::MagicLevelSource* outputMeter = nullptr;
    foleys::MagicPlotSource* oscilloscope = nullptr;
    foleys::MagicPlotSource* analyzer = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubtractiveSynthAudioProcessor)
};
