/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MicroModulationAudioProcessorEditor::MicroModulationAudioProcessorEditor (MicroModulationAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), fileComponent(p.midiProcessor.scale, juce::Colours::palegreen),
modulationComponent(juce::Colours::blueviolet)
{
//    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
//    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
//    addAndMakeVisible(gainSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvst, "GAIN", gainSlider);
//    textButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvst, "BUTTON", textButton);
    
    
    addAndMakeVisible(fileComponent);
    addAndMakeVisible(modulationComponent);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

MicroModulationAudioProcessorEditor::~MicroModulationAudioProcessorEditor()
{
}

//==============================================================================
void MicroModulationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MicroModulationAudioProcessorEditor::resized()
{
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows    = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };

    grid.items = { juce::GridItem (fileComponent), juce::GridItem (modulationComponent) };

    grid.performLayout (getLocalBounds());
    
}
