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
    std::vector<int> getMapping(){return this->mapping;}
    signed char getMapping(size_t scaleDegree){return this->mapping.at(scaleDegree);}
    
    std::pair<signed char, signed char> getRangeToRetune(){return this->rangeToRetune;}
    std::pair<signed char, float> getMiddleNoteFreqPair(){return this->middleNoteFreqPair;}
    std::pair<signed char, float> getRefererenceMidiFreqPair(){return this->referenceMidiFreqPair;}
    size_t getFormalOctaveScaleDegree(){return this->formalOctaveScaleDegree;}
    // ==============================================================================
    //
    // ==============================================================================
    
    
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
    //parameters.
    std::pair<signed char, signed char> rangeToRetune;
    std::pair<signed char, float> middleNoteFreqPair;
    std::pair<signed char,float> referenceMidiFreqPair;
    size_t formalOctaveScaleDegree;
    std::vector<int> mapping;
    //functions
    int getScaleDegree(signed char midiNoteNum);
    int getOctave(signed char midiNoteNum);
    void calcMiddleNoteFreq();
    
    
    //defaults
    static struct defaults{
        const std::pair<signed char, signed char> rangeToRetune = std::pair<signed char, signed char>(0,127); //standard midi range
        const std::pair<signed char, float> middleNoteFreqPair = std::pair<signed char, float>(60, 261.62556530059863467784999352330470136445386034203983086418048943);//middle c. freq computed with wolfram alpha formula: e^(300 * ln(2) / 1200)*220.
        const std::pair<signed char,float> referenceMidiFreqPair = std::pair<signed char,float>(69, 440.0f); //A is mapped to 440Hz.
    } defaults;
    
    //functions
    
};
