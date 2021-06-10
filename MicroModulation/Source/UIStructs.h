/*
  ==============================================================================

    UIStructs.h
    Created: 28 May 2021 12:44:41pm
    Author:  Willow Weiner

  ==============================================================================
*/

#pragma once

#include <vector>

#include "Scale.h"

namespace ui_structs{
struct FileLoadingComponent : public juce::Component, public juce::Button::Listener
{
    //TODO: may need to change Scale s to pass by reference
    FileLoadingComponent(Scale &s, juce::Colour c) : scale(s), backgroundColour(c), loadSclButton("Load .scl file."), loadKbmButton("Load .kbm file."), loadedSclLabel("", "No .scl loaded"), loadedKbmLabel("", "No .kbm loaded.")
    {
        addAndMakeVisible(loadSclButton);
        loadSclButton.addListener(this);
        addAndMakeVisible(loadKbmButton);
        loadKbmButton.addListener(this);
        
        addAndMakeVisible(loadedSclLabel);
        addAndMakeVisible(loadedKbmLabel);
    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (backgroundColour);
    }
    
    void resized() override
    {
        juce::FlexBox fb;
        
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;
        
        fb.items.add(juce::FlexItem(loadSclButton)
                     .withMinWidth(80.0f).withMinHeight(20.0f).withMaxHeight(50.0f));
        fb.items.add(juce::FlexItem(loadedSclLabel)
                     .withMinWidth(80.0f).withMinHeight(20.0f).withMaxHeight(50.0f));
        fb.items.add(juce::FlexItem(loadKbmButton)
                     .withMinWidth(80.0f).withMinHeight(20.0f).withMaxHeight(50.0f));
        fb.items.add(juce::FlexItem(loadedKbmLabel)
                     .withMinWidth(80.0f).withMinHeight(20.0f).withMaxHeight(50.0f));
        
        fb.performLayout(getLocalBounds().toFloat());
    }
    
    void buttonClicked (juce::Button* button) override
    {
        if(button == &loadSclButton || button == &loadKbmButton) {
            juce::FileChooser chooser ("Pick a file.");
            if (chooser.browseForFileToOpen()){
                auto result = chooser.getResult();
                
                if(button == &loadSclButton){
                    if(scale.loadSclFile(result)){
                        //if a new .scl file is loaded, set loadedSclLabel text to file name.
                        loadedSclLabel.setText(result.getFileNameWithoutExtension(), juce::NotificationType::dontSendNotification);
                    }
                } else if(button == &loadKbmButton) {
                    if(scale.getNotes().size() > 0) {// .kbm file can only be chosen if a .scl file is loaded
                        //if a new .kbm file is loaded, set loadedKbmFile text to file name.
                        if(scale.loadKbmFile(chooser.getResult())) {
                            loadedKbmLabel.setText(result.getFileNameWithoutExtension(), juce::NotificationType::dontSendNotification);
                        }
                    }
                }
            }
        }
    }
private:
    Scale& scale;

    juce::Colour backgroundColour;
    
    juce::TextButton loadSclButton;
    juce::Label loadedSclLabel;
    
    juce::TextButton loadKbmButton;
    juce::Label loadedKbmLabel;
    
};

struct ModulationControlsComponent : public juce::Component, public juce::ValueTree::Listener
{
public:
    ModulationControlsComponent(juce::Colour c, MidiProcessor& mp): backgroundColour(c), midiProcessorVals(mp.midiProcessorValues), setCenterButton("", juce::Colours::black, juce::Colours::red, juce::Colours::green), curCenterLabel("", "center: NaN"), curPivotLabel("", "pivot: NaN"), lastMidiNoteLabel("","last note: NaN")
    {
//        labels.push_back(&curCenterLabel);
//        labels.push_back(&curPivotLabel);
//        labels.push_back(&lastMidiNoteLabel);
//
        addAndMakeVisible(curCenterLabel);
        addAndMakeVisible(curPivotLabel);
        addAndMakeVisible(lastMidiNoteLabel);
        
        midiProcessorVals.addListener(this);

    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (backgroundColour);
    }

    void resized() override
    {
        juce::FlexBox fb;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        
        fb.items.add(juce::FlexItem(curCenterLabel)
                     .withMinWidth(50.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(curPivotLabel)
                     .withMinWidth(50.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(lastMidiNoteLabel)
                     .withMinWidth(50.f)
                     .withMinHeight(50.0f));

//        for(auto* l : labels) {
//            fb.items.add(juce::FlexItem(*l).withMinWidth(50.f).withMinHeight(50.0f));
//        }
        //fb.items.add(juce::FlexItem(setCenterButton).withMinWidth(50.f).withMinHeight(50.0f));
        
        fb.performLayout(getLocalBounds().toFloat());

    }
    
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override
    {
        const juce::MessageManagerLock mmLock;

        if(tree.getType() == juce::Identifier("midiProcessor"))
        {
            if(property == juce::Identifier("lastNotePlayed"))
            {
                lastMidiNoteLabel.setText(tree.getProperty(property), juce::NotificationType::dontSendNotification);
                
            }
        }
    }
    
private:
    juce::Colour backgroundColour;

    juce::ShapeButton setCenterButton;
    juce::Label curCenterLabel;

  //  juce::Button setPivotButton;
    juce::Label curPivotLabel;

    juce::Label lastMidiNoteLabel;

   // juce::Button modulateButton;
    
    std::vector<juce::Label*> labels;
    
    
    juce::ValueTree midiProcessorVals;
};

} // end namespace ui_structs
