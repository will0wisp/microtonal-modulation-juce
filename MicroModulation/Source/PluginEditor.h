/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "UIStructs.h"

//==============================================================================
/**
*/
class MicroModulationAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MicroModulationAudioProcessorEditor (MicroModulationAudioProcessor&);
    ~MicroModulationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MicroModulationAudioProcessor& audioProcessor;

    
    ui_structs::FileLoadingComponent fileComponent;
    ui_structs::ModulationControlsComponent modulationComponent;
    
    //currently unused, from AudioProcessorValueTreeState tutorial.
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    juce::TextButton textButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> textButtonAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MicroModulationAudioProcessorEditor)
};
