/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanningLawsAudioProcessor::PanningLawsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PanningLawsAudioProcessor::~PanningLawsAudioProcessor()
{
}

//==============================================================================
const juce::String PanningLawsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PanningLawsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PanningLawsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PanningLawsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PanningLawsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PanningLawsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PanningLawsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PanningLawsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PanningLawsAudioProcessor::getProgramName (int index)
{
    return {};
}

void PanningLawsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PanningLawsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PanningLawsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PanningLawsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PanningLawsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // Map slider value (0 - 1) to angle (0 - pi/2)
        // cos(pi/2) = 0, sin(pi/2) = 1
        // cos(0)    = 1, sin(0)    = 0
        const float theta = panPos * juce::MathConstants<float>::halfPi;

        if (panLaw == 0) LinearPanning(buffer, theta);
        else if (panLaw == 1) ConstantPowerPanning(buffer, theta);
        else dBLawPanning(buffer, theta);

    }
}


// Linear panning
// L(theta) = (pi/2 - theta) * (2 / pi)
// R(theta) = theta * (2 / pi)
void PanningLawsAudioProcessor::LinearPanning(juce::AudioBuffer<float>& buffer, float theta)
{
    float gainL = (juce::MathConstants<float>::halfPi - theta) * (2.0f / juce::MathConstants<float>::pi);
    float gainR = theta * (2.0f / juce::MathConstants<float>::pi);
    buffer.applyGain(0, 0, buffer.getNumSamples(), gainL);
    buffer.applyGain(1, 0, buffer.getNumSamples(), gainR);
}

// Constant power
// (L(theta) = cos(theta)) and (R(theta) = sin(theta)
void PanningLawsAudioProcessor::ConstantPowerPanning(juce::AudioBuffer<float>& buffer, float theta)
{
    float gainL = std::cos(theta);
    float gainR = std::sin(theta);
    buffer.applyGain(0, 0, buffer.getNumSamples(), gainL);
    buffer.applyGain(1, 0, buffer.getNumSamples(), gainR);
}

// -4.5 db Law
// L(theta) = sqrt(pi/2 - theta) * (2/pi) * cos(theta)
// R(theta) = sqrt(theta * 2/pi) * sin(theta)
void PanningLawsAudioProcessor::dBLawPanning(juce::AudioBuffer<float>& buffer, float theta)
{
    float gainL = std::sqrt((juce::MathConstants<float>::halfPi - theta) * (2.0f / juce::MathConstants<float>::pi ) * std::cos(theta));
    float gainR = std::sqrt((theta) * (2.0f / juce::MathConstants<float>::pi ) * std::sin(theta));
    buffer.applyGain(0, 0, buffer.getNumSamples(), gainL);
    buffer.applyGain(1, 0, buffer.getNumSamples(), gainR);
}

//==============================================================================
bool PanningLawsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PanningLawsAudioProcessor::createEditor()
{
    return new PanningLawsAudioProcessorEditor (*this);
}

//==============================================================================
void PanningLawsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PanningLawsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PanningLawsAudioProcessor();
}
