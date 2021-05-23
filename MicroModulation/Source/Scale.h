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

class Scale
{
public:
    Scale(std::string sclPath);
    Scale(std::string sclPath, std::string kbmPath);
    // ==============================================================================
    // Static Stuff
    // ==============================================================================
    static float centsToRatio(float cents){
        return std::pow(M_E, (cents * std::log(2.0) / 1200) );
    }
    
    static std::string strToTmpFilePath(std::string fileContents);
    // ==============================================================================
    // Getters and Setters
    // ==============================================================================
    std::string getDescription() {return this->description;}
    void setDescription(std::string d){this->description = d;}
    
    /**
     @param midiNote the midiNote coming in. Ranges [0,127];
     @return calculated frequency based on midiNote
     */
    float getFreq(char midiNote);
    void modulate(char pivot, char center);
    
    
    bool loadSclFile(std::string sclPath);
    bool loadKbmFile(std::string kbmPath);
private:
    std::string description;
    std::vector<float> notes;
    
    KeyboardMap map;
};
