#include "JuceHeader.h"
#include "BinaryData.h"
#include "PluginProcessor.h"
#include "ParametersManager.h"

//==============================================================================
SubtractiveSynthAudioProcessor::SubtractiveSynthAudioProcessor() : foleys::MagicProcessor(
        juce::AudioProcessor::BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
        treeState (*this, nullptr, ProjectInfo::projectName, ParametersManager::createParameterLayout())
{
    // Magic Sound Visualization parameters
    outputMeter = magicState.createAndAddObject<foleys::MagicLevelSource>("output");
    analyzer = magicState.createAndAddObject<foleys::MagicAnalyser>("analyzer");
    oscilloscope = magicState.createAndAddObject<foleys::MagicOscilloscope>("oscilloscope");

    synthesiser.addSound(new OranJam::SynthSound(treeState));
    for (auto i = 0; i < OranJam::numVoices; i++)
    {
        synthesiser.addVoice(new OranJam::SynthVoice());
    }

    magicState.setApplicationSettingsFile(juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                                  .getChildFile(ProjectInfo::companyName)
                                                  .getChildFile(ProjectInfo::projectName + juce::String(".settings")));
}

//==============================================================================
void SubtractiveSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Magic Sound Visualization
    // Set up the meter. Modifying the last parameter sets the refresh rate
    outputMeter->setupSource(getTotalNumOutputChannels(), sampleRate, 50);
    // This is the default method, it's possible to implement custom ones, for ex for single channels
    magicState.prepareToPlay(sampleRate, samplesPerBlock);

    synthesiser.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void SubtractiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SubtractiveSynthAudioProcessor::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout& layouts) const
{
    // This synth only supports mono or stereo.
    return (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
            || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo());
}

double SubtractiveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

void SubtractiveSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    /* In case we have more outputs than inputs, this code clears any output channels that didn't contain input data,
     * (because these aren't guaranteed to be empty - they may contain garbage). This is here to avoid people getting
     * screaming feedback when they first compile a plugin, but obviously you don't need to keep this code if your
     * algorithm always overwrites all the output channels. */
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Magic state MIDI processing
    magicState.processMidiBuffer(midiMessages, buffer.getNumSamples());

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Magic State attach sound visualizers
    outputMeter->pushSamples(buffer);
    analyzer->pushSamples(buffer);
    oscilloscope->pushSamples(buffer);
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubtractiveSynthAudioProcessor();
}

//Loads the xml that defines the Visual layout of the components inside the GUI
juce::AudioProcessorEditor* SubtractiveSynthAudioProcessor::createEditor()
{
    magicState.setGuiValueTree(BinaryData::magic_xml, BinaryData::magic_xmlSize);
    return new foleys::MagicPluginEditor(magicState);
}


