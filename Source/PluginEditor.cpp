/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanningLawsAudioProcessorEditor::PanningLawsAudioProcessorEditor (PanningLawsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // set up UI
    addAndMakeVisible(panSlider);
    panSlider.setRange(0.0, 1.0);
    panSlider.setValue(0.5);
    panSlider.onValueChange = [this] {
        audioProcessor.panPos = panSlider.getValue();
    };
    
    addAndMakeVisible(lawSelector);
    lawSelector.addItem("Linear", 1);
    lawSelector.addItem("Constant Power", 2);
    lawSelector.addItem("-4.5dB", 3);
    lawSelector.setSelectedId(1);
    lawSelector.onChange = [this] {
        audioProcessor.panLaw = lawSelector.getSelectedId() - 1;
    };
}

PanningLawsAudioProcessorEditor::~PanningLawsAudioProcessorEditor()
{
}

//==============================================================================
void PanningLawsAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText("Pan", 50, 70, 300, 30, juce::Justification::centred, 1);
    g.drawFittedText("Panning Law", 100, 155, 200, 25, juce::Justification::centred, 1);
    
    
}

void PanningLawsAudioProcessorEditor::resized()
{
    panSlider.setBounds(50, 100, 300, 50);
    lawSelector.setBounds(100, 180, 200, 30);
}
