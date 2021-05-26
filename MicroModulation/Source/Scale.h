/*
 ==============================================================================

 Scale.h
 
   
 This class provides a wrapper for the Scala '.scl' file type.
 It allows users to use custom scales, based on ratios and 
    
 
 Created: 21 May 2021 10:03:02pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <cmath>
#include <string>

#include "KeyboardMap.h"

//TODO: Add complete documentation
class Scale
{
public:
    Scale();
    Scale(std::string sclPath);
    Scale(std::string sclPath, std::string kbmPath);
    
    struct StoredScale
    {
        StoredScale(Scale& s)
            :d(s.getDescription()), n(s.getNotes()), kbm(s.getKeyboardMap())
        {}
        
        bool equals(Scale& s)
        {
            return d == s.description
            && n == s.notes;
                //&& kbm == s.kbm;
        }
        std::string d;
        std::vector<float> n;
        KeyboardMap kbm;
    };
    // ==============================================================================
    // Static Stuff
    // ==============================================================================
    static float centsToRatio(float cents){
        return std::pow(M_E, (cents * std::log(2.0) / 1200.0) );
    }
    
    // ==============================================================================
    // Getters and Setters
    // ==============================================================================
    std::string getDescription(){return this->description;}
    void setDescription(std::string d){this->description = d;}
    
    std::vector<float> getNotes(){return this->notes;}
    void setNotes(std::vector<float> n){this->notes = n;}
    
    KeyboardMap getKeyboardMap(){return this->kbm;}
    
    // ==============================================================================
    // File loading
    // ==============================================================================
    
    /**
     @param midiNote the midiNote coming in. Ranges [0,127];
     @return calculated frequency based on midiNote
     */
   // virtual float getFreq(signed char midiNote);
   // virtual void modulate();
    
    bool loadSclFile(std::string sclPath);
    /*
     Loads a scale based on a .scl file stored in a string.
     @param sclString a string that is formatted like a .scl file. to be loaded
     */
    bool loadSclString(std::string sclString);

    bool loadKbmFile(std::string kbmPath);
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
    std::string description;
    std::vector<float> notes;
    KeyboardMap kbm;

};
