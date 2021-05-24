/*
 ==============================================================================
 
 KeyboardMap.cpp
 Created: 23 May 2021 5:06:31pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#include <cstdio>
#include <fstream>

#include "KeyboardMap.h"

bool KeyboardMap::loadKbmFile(std::string kbmPath)
{
    std::ifstream kbmFile(kbmPath);
    std::string line;
    if(kbmFile.is_open())
    {
        int lineNum = 0;
        try
        {
            mapping.clear();
            int mappingSize;
            while(getline(kbmFile, line))
            {
                line = removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    if(++lineNum > 7)
                    {
                        mapping.push_back(std::stoi(line));
                        if(mapping.back() <= 0) return false;
                    }
                    else if(lineNum == 1) //then line = the size of map. after how many keys the pattern repeats.
                    {
                        mappingSize = std::stoi(line);
                        if(mappingSize <= 0 ) return false;
                    }
                    else if(lineNum == 2) //first midi note number to retune
                    {
                        rangeToRetune.first = std::stoi(line);
                    }
                    else if(lineNum == 3) //last midi note number to retune
                    {
                        rangeToRetune.second = std::stoi(line);
                    }
                    else if(lineNum == 4) //middle note
                    {
                        middleNoteFreqPair.first = std::stoi(line);
                        if(middleNoteFreqPair.first < 0 || middleNoteFreqPair.first > 127) return false;
                    }
                    else if(lineNum == 5) //midi reference note
                    {
                        referenceMidiFreqPair.first = std::stoi(line);
                    }
                    else if(lineNum == 6) //frequence of reference midi note
                    {
                        referenceMidiFreqPair.second = std::stof(line);
                        if(referenceMidiFreqPair.second <= 0.0) return false;
                    }
                    else if(lineNum == 7) //scale number to consider formal octave.
                    {
                        formalOctaveScaleDegree = std::stoi(line);
                        if(formalOctaveScaleDegree < 0 || notes.size() < formalOctaveScaleDegree) return false;
                    }
                }
            }
            kbmFile.close();
            
            if(lineNum < 7) return false; //in this case, not all meta values were read.
            
            //fill rest of mappings with -1
            for(size_t i = mapping.size(); i < mappingSize ; i++)
            {
                mapping.push_back(-1);
            }
            
            if(mapping.size() != mappingSize) return false; //wrong mapping size
            
            calcMiddleNoteFreq();
        }
        catch (...) { return false; } // if formatted incorrectly, return false
        return true;
    }
    else return false; // if file didn't open, return false;
}
bool KeyboardMap::loadKbmString(std::string kbmString)
{
    std::string tmpfilePath = makeAndWriteTmpFile(kbmString);
    
    bool output = loadKbmFile(tmpfilePath);
    remove(tmpfilePath.c_str());
    return output;
}

//TODO: test this
float KeyboardMap::getFreq(signed char midiNoteNum)
{
    return notes.at(getScaleDegree(midiNoteNum)) //the ratio for midinote
    * pow( notes.at(formalOctaveScaleDegree), getOctave(midiNoteNum) ) //takes care of octave shift
    * middleNoteFreqPair.second;
}

//TODO: write this
//TODO: test this
void KeyboardMap::modulate(signed char center, signed char pivot)
{
    
}

//TODO: test this
int KeyboardMap::getScaleDegree(signed char midiNoteNum)
{
    assert(mapping.size() > 0);
    assert(midiNoteNum >= 0);
    int index;
    if(midiNoteNum >= middleNoteFreqPair.first)
    {
        index = (midiNoteNum - middleNoteFreqPair.first) % mapping.size();
    }
    else{
        index = mapping.size() - (middleNoteFreqPair.first - midiNoteNum) % mapping.size();
        if(index == mapping.size()) index = 0;
    }
    return mapping.at(index);
    
}

//TODO: write this
//TODO: test this
int KeyboardMap::getOctave(signed char midiNoteNum)
{
    //  (midiNoteNum - middleNoteFreqPair.first) % mapping.size();
    return -1;
}


//TODO: write this
//TODO: test this
void KeyboardMap::calcMiddleNoteFreq()
{
    
}
