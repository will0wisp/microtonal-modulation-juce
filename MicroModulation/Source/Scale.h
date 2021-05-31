/*
 ==============================================================================

 Scale.h
 
   
 This class provides a wrapper for the Scala '.scl' file type.
 It allows users to use custom scales, based on ratios and 
    
 
 Created: 21 May 2021 10:03:02pm
 Author:  Willow Weiner
 
 =============================================================================
 */

#pragma once

#include <cmath>
#include <string>

#include "JuceHeader.h"

#include "KeyboardMap.h"

//TODO: Add complete documentation
class Scale
{
public:
    Scale(juce::UndoManager& um);
    Scale(juce::UndoManager& um, std::string sclPath);
    Scale(juce::UndoManager& um, std::string sclPath, std::string kbmPath);
    
    juce::ValueTree scaleValues;
    // ==============================================================================
    // Static Stuff
    // ==============================================================================
    static float centsToRatio(float cents){
        return std::pow(M_E, (cents * std::log(2.0) / 1200.0) );
    }
    
    // ==============================================================================
    // Getters and Setters
    // ==============================================================================

    juce::Array<juce::var>& getNotes(){
        jassert(scaleValues.hasProperty("notes"));
        jassert(scaleValues.getProperty("notes").isArray());
        return *(scaleValues.getProperty("notes").getArray());
    }
    float getNote(int i) {
        if(i >= getNotes().size() || i < 0) return -1;
        return getNotes().getUnchecked(i);
    }
    void setNotes(juce::Array<juce::var> newNotes) {
        jassert(scaleValues.hasProperty("notes"));
        scaleValues.getPropertyAsValue("notes", &undoManager).setValue(juce::var(newNotes));
    }
    
    
    std::string getDescription(){ return scaleValues.getProperty("description").toString().toStdString();}

    KeyboardMap getKeyboardMap(){return this->kbm;}
    void setKeyboardMap(KeyboardMap kb){this->kbm = kb;}

    // ==============================================================================
    // File loading
    // ==============================================================================
    bool loadSclFile(std::string sclPath);
    bool loadSclFile(juce::File sclFile);
    /*
     Loads a scale based on a .scl file stored in a string.
     @param sclString a string that is formatted like a .scl file. to be loaded
     */
    bool loadSclString(std::string sclString);

    bool loadKbmFile(std::string kbmPath);
    bool loadKbmFile(juce::File kbmFile);
    bool loadKbmString(std::string kbmString);

    
    
    /*
     Returns the frequency that should be played back.
     @param midiNoteNum the midiNote number.
     @return the frequncy that is associated with that midi note number.
     */
    float getFreq(signed char midiNoteNum);
    /*
     Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
     @param center midino
     @param pivot
     */
    void modulate(signed char center, signed char pivot);
    
    
private:
    
    juce::UndoManager& undoManager;
    KeyboardMap kbm;
    void calcFundamentalFreq();
};
