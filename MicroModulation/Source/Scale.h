/*
 ==============================================================================

 Scale.h
 
   
 This class acts as a wrapper for the Scala '.scl' file type.
 It allows users to use custom scales with intervals based on ratios or specified in cents of a semitone.
 This class also translates MIDI note numbers into frequency information, with help from the KeyboardMap class.
    
 
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
    //TODO: move this to utils.h, maybe? It may be able to just be a private function, as well.
    //It is something that may be helpful to have, which is why I have put it here.
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
    //void setKeyboardMap(KeyboardMap kb){this->kbm = kb;}

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

    bool hasSclLoaded(){return hasScl;}
    
    /*
     Returns the frequency that should be played back.
     @param midiNoteNum the midiNote number.
     @return the frequncy that is associated with that midi note number.
     */
    float getFreq(juce::int8 midiNoteNum);
    /*
     Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
     @param center midino
     @param pivot
     */
    void modulate(juce::int8 center, juce::int8 pivot);
    
private:
    
    juce::UndoManager& undoManager;
    KeyboardMap kbm;
    void calcFundamentalFreq();
    bool hasScl; //TODO: conver this to a scaleValues property
    
    juce::Array<float> calculatedFreqs; //stores frequencyies that have already been calculated so that getFreq() is more efficient.
    void initCalculatedFreqs();
    bool freqHasBeenCalculated(juce::int8 midiNoteNum);

};
