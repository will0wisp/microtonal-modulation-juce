/*
 ==============================================================================
 
 KeyboardMap.h
 
 This class provides a wrapper for the Scala '.kbm' file type.
 It allows users to configure which input midi note numbers are matched to which Scale Degree.
 See more here: http://www.huygens-fokker.org/scala/help.htm#mappings 
 
 
 Note, throught this project, char's are used to represent midi notes, as 8 bit values.
 Created: 21 May 2021 10:20:59pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <string>
#include <vector>

#include "Scale.h"

class KeyboardMap: public Scale
{
public:
    KeyboardMap(){}
    KeyboardMap(std::string kbmPath);
    KeyboardMap(int scaleLength);
    
    // ==============================================================================
    // Getters and Setters
    // ==============================================================================
    std::vector<signed char> getMapping(){return this->mapping;}
    signed char getMapping(size_t scaleDegree){return this->mapping.at(scaleDegree);}
    
    std::pair<signed char, signed char> getRangeToRetune(){return this->rangeToRetune;}
    signed char getMiddleNote(){return this->middleNote;}
    std::pair<signed char, float> getRefererenceMidiFreqPair(){return this->referenceMidiFreqPair;}
    size_t getFormalOctaveScaleDegree(){return this->formalOctaveScaleDegree;}
    // ==============================================================================
    //
    // ==============================================================================
    
    
    bool loadKbmFile(std::string kbmPath);
    bool loadKbmString(std::string kbmString);
    signed char calcScaleDegree(signed char midiNoteNum);
    
    
private:
    //parameters.
    std::pair<signed char, signed char> rangeToRetune;
    char middleNote;
    std::pair<signed char,float> referenceMidiFreqPair;
    size_t formalOctaveScaleDegree;
    std::vector<signed char> mapping;
    //variables
    
    
    //defaults
    static struct defaults{
        const std::pair<signed char, signed char> rangeToRetune = std::pair<signed char, signed char>(0,127); //standard midi range
        const char middleNote = 60; //middle c
        const std::pair<signed char,float> referenceMidiFreqPair = std::pair<signed char,float>(69, 440.0f); //A is mapped to 440Hz.;
    } defaults;
    
    //functions
    
};
