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

class KeyboardMap
{
public:
    KeyboardMap(){}
    KeyboardMap(std::string kbmPath);
    KeyboardMap(int scaleLength);
//    KeyboardMap(Scale s);

    bool loadKbmFile(std::string kbmPath);
    signed char getScaleDegree(signed char midiNoteNum);
private:
    //variables.
    std::pair<signed char, signed char> rangeToRetune;
    char middleNote;
    std::pair<signed char,float> referenceMidiFreqPair;
    int formalOctaveScaleDegree;
    std::vector<signed char> mapping;
    
    //defaults
    static struct defaults{
        const std::pair<signed char, signed char> rangeToRetune = std::pair<signed char, signed char>(0,127); //standard midi range
        const char middleNote = 60; //middle c
        const std::pair<signed char,float> referenceMidiFreqPair = std::pair<signed char,float>(69, 440.0f); //A is mapped to 440Hz.;
    } defaults;
    
    //functions
    
};
