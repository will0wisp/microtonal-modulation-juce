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
/*
 A UI Component that displays the real-time value of a juce::Value alongside a text discription.
 The text discription is to the left of the value.
 */
class ValueLabel : public juce::Component, public juce::Value::Listener
{
public:
    ValueLabel(){}
    ValueLabel(const juce::String text, const juce::Value value, const juce::Colour backgroundColour): text(text), value(value), backgroundColour(backgroundColour)
    {
        this->value.addListener(this);
        
        textLabel.attachToComponent(&valueLabel, true);
        setText(text);
        
        valueLabel.setText(initialValueText, juce::NotificationType::dontSendNotification);
        
        addAndMakeVisible(textLabel);
        addAndMakeVisible(valueLabel);
    }
    void setText(juce::String text)
    {
        this->text = text;
        textLabel.setText(text, juce::NotificationType::dontSendNotification);
    }
    void valueChanged(juce::Value& value) override
    {
        valueLabel.setText(value.toString(), juce::NotificationType::sendNotification);
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
        
        fb.items.add(juce::FlexItem(textLabel)
                     .withMinWidth(50.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(valueLabel)
                     .withMinWidth(50.f)
                     .withMinHeight(50.0f));
        fb.performLayout(getLocalBounds().toFloat());
    }

private:
    const juce::String initialValueText = "NA";
    
    juce::String text;
    juce::Label textLabel;
        
    juce::Value value;
    juce::Label valueLabel;
    
    juce::Colour backgroundColour;
};


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

struct ModulationControlsComponent : public juce::Component, juce::Button::Listener
{
public:
    ModulationControlsComponent(juce::Colour c, MidiProcessor& mp):
    midiProcessor(mp),
    backgroundColour(c),
    lastMidiNoteLabel("Last note: ", mp.midiProcessorValues.getPropertyAsValue(juce::Identifier("lastNotePlayed"), nullptr), c),
    curCenterLabel("Center: ", mp.midiProcessorValues.getPropertyAsValue(juce::Identifier("curCenter"), nullptr), c),
    curPivotLabel("Pivot: ", mp.midiProcessorValues.getPropertyAsValue(juce::Identifier("curPivot"), nullptr), c),
    setCenterButton("Set Center"), setPivotButton("Set Pivot")
    {
        setCenterButton.addListener(this);
        setPivotButton.addListener(this);
        
        addAndMakeVisible(lastMidiNoteLabel);
        addAndMakeVisible(curCenterLabel);
        addAndMakeVisible(curPivotLabel);
        
        addAndMakeVisible(setCenterButton);
        addAndMakeVisible(setPivotButton);

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


        fb.items.add(juce::FlexItem(lastMidiNoteLabel)
                     .withMinWidth(100.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(curCenterLabel)
                     .withMinWidth(100.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(curPivotLabel)
                     .withMinWidth(100.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(setCenterButton)
                     .withMinWidth(100.f)
                     .withMinHeight(50.0f));
        fb.items.add(juce::FlexItem(setPivotButton)
                     .withMinWidth(100.f)
                     .withMinHeight(50.0f));
        fb.performLayout(getLocalBounds().toFloat());

    }
    
    void buttonClicked (juce::Button* button) override
    {
        if(button == &setCenterButton) midiProcessor.setCenter();
        if(button == &setPivotButton) midiProcessor.setPivot();
    }
    
private:
    MidiProcessor& midiProcessor;
    juce::Colour backgroundColour;
    
    ValueLabel lastMidiNoteLabel;
    ValueLabel curCenterLabel;
    ValueLabel curPivotLabel;
    
    juce::TextButton setCenterButton;
    juce::TextButton setPivotButton;
};

} // end namespace ui_structs
