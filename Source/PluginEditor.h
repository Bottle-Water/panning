/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PanningLawsAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PanningLawsAudioProcessorEditor (PanningLawsAudioProcessor&);
    ~PanningLawsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PanningLawsAudioProcessor& audioProcessor;
    juce::Slider panSlider;
    juce::ComboBox lawSelector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanningLawsAudioProcessorEditor)
};
