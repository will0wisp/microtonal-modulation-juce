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


class KeyboardMap
{
public:
    KeyboardMap();
    KeyboardMap(int scaleLength);
    KeyboardMap(int scaleLength, std::string kbmPath);

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
     Returns the scale degree for a given midi note
     @param midiNoteNum the midi note number. on [0,127]
     @return the associated scale degree (of scale stored in this->notes)
     */
    int getScaleDegree(signed char midiNoteNum);
    int getOctave(signed char midiNoteNum);
    
    /*
     Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
     @param center midino
     @param pivot
     */
    void modulate(signed char center, signed char pivot);
    
private:
    size_t scaleLength;
    //.kbm file parameters.
    std::pair<signed char, signed char> rangeToRetune;
    std::pair<signed char, float> middleNoteFreqPair;
    std::pair<signed char,float> referenceMidiFreqPair;
    size_t formalOctaveScaleDegree;
    std::vector<int> mapping;
    //functions
    
    void calcMiddleNoteFreq();
    
    //defaults
    static struct defaults{
        const std::pair<signed char, signed char> rangeToRetune = std::pair<signed char, signed char>(0,127); //standard midi range
        const std::pair<signed char, float> middleNoteFreqPair = std::pair<signed char, float>(60, 261.62556530059863467784999352330470136445386034203983086418048943);//middle c. freq computed with wolfram alpha formula: e^(300 * ln(2) / 1200)*220.
        const std::pair<signed char,float> referenceMidiFreqPair = std::pair<signed char,float>(69, 440.0f); //A is mapped to 440Hz.
    } defaults;
    
    //functions
    
};
