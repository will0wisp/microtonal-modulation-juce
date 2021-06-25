/*
 ==============================================================================
 
 KeyboardMap.h
 
 This class acts as a wrapper for the Scala '.kbm' file type.
 It allows users to configure which input midi note numbers are matched to which Scale Degree.
 See more here: http://www.huygens-fokker.org/scala/help.htm#mappings 
 
 
 Created: 21 May 2021 10:20:59pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <string>
#include <vector>

#include "JuceHeader.h"

#include "Identifiers.h"

class KeyboardMap
{
public:
    KeyboardMap(juce::UndoManager& um);
    KeyboardMap(juce::UndoManager& um, int scaleLength);
    KeyboardMap(juce::UndoManager& um, int scaleLength, std::string kbmPath);

    juce::ValueTree keyboardMapValues;
        
    /**
     Sets the mapping to a default state, based on the length of the scale.
     The default state is a linear, one-note-per-key mapping. For example, if we define the center midi note =: 'c',
     we get the default mapping:
     
     MIDI NOTE NUM         -> SCALE DEGREE
     ...
     c- scaleLength             -> 0
     c-scaleLength + 1        -> 1
     ...
     c-2                               -> scaleLength - 2
     c-1                               -> scaleLength - 1
     c                                  -> 0
     c+1                              -> 1
     c+2                              -> 2
     ...
     c+ scaleLength - 1      -> scaleLength - 1
     c+ scaleLength           -> 0
     ...
     
     @param scaleLength. The number of notes in the scale this map is serviceing.
     */
    void setToDefaultMapping(int scaleLength);
    // ==============================================================================
    // Getters and Setters
    // ==============================================================================
    
    //TODO: move these definitions to .cpp file.
    juce::Array<juce::var>& getMapping(){
        jassert(keyboardMapValues.hasProperty(IDs::keyboardMapping));
        jassert(keyboardMapValues.getProperty(IDs::keyboardMapping).isArray());
        return *(keyboardMapValues.getProperty(IDs::keyboardMapping).getArray());
    }
    int getMapping(int midiNote){
        if(midiNote >= getMapping().size() || midiNote < 0) return -1;
        return getMapping().getUnchecked(midiNote);
    }
    
    int getRetuneRangeUpperBound(){return keyboardMapValues.getProperty(IDs::retuneRangeUpperBound);}
    int getRetuneRangeLowerBound(){return keyboardMapValues.getProperty(IDs::retuneRangeLowerBound);}
    int getMiddleNote(){return keyboardMapValues.getProperty(IDs::middleNote);}
    int getReferenceMidiNote(){return keyboardMapValues.getProperty(IDs::referenceNote);}
    float getReferenceFreq(){return keyboardMapValues.getProperty(IDs::referenceFreq);}
    int getFormalOctaveScaleDegree(){return keyboardMapValues.getProperty(IDs::formalOctaveScaleDegree);}
    
    // ==============================================================================
    // File Parsing
    // ==============================================================================
    bool loadKbmFile(std::string kbmPath);
    bool loadKbmString(std::string kbmString);
    
    
    // ==============================================================================
    // Main functionality
    // ==============================================================================
    /**
     @param midiNoteNum. The juce::int8 midi note number.
     @return The int index associated to midiNoteNum in the keyboardMapping.
     */
    int getMappingIndex(juce::int8 midiNoteNum);
    /**
     * Returns the scale degree for a given midi note
     * @param midiNoteNum the midi note number. on [0,127]
     * @return the associated scale degree (of scale stored in this->notes)
     */
    int getScaleDegree(juce::int8 midiNoteNum);
    /**
     Returns how how many octaves above (positive) or below (negative) a note is compared to the middleNote.
     @param
     */
    int getOctave(juce::int8 midiNoteNum);
    
    /**
     Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
     @param center midino
     @param pivot
     */
    void modulate(juce::int8 center, juce::int8 pivot);
    
private:
    juce::UndoManager& undoManager;
    
    void setDefaultValues();
};
