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
#include "utils.h"

KeyboardMap::KeyboardMap(int sclLength)
    :scaleLength(sclLength), rangeToRetune(0,127), middleNoteFreqPair(60, 261.625565), referenceMidiFreqPair(69, 440.0), formalOctaveScaleDegree(sclLength-1)
{
    for(int i = 0; i < sclLength; i++)
    {
        mapping.push_back(i);
    }
}
KeyboardMap::KeyboardMap(int sclLength, std::string kbmPath)
    :scaleLength(sclLength)
{
    loadKbmFile(kbmPath);
}

bool KeyboardMap::loadKbmFile(std::string kbmPath)
{
    std::ifstream kbmFile(kbmPath);
    std::string line;
    if(kbmFile.is_open())
    {
        StoredKeyboardMap beforeRead = StoredKeyboardMap(*this);
        bool fileReadCorrectly = true;
        
        int lineNum = 0;
        int mappingSize;

        try
        {
            mapping.clear();
            while(getline(kbmFile, line))
            {
                line = utils::removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    switch(++lineNum)
                    {
                        case 1: //line = the size of map. after how many keys the pattern repeats.
                            mappingSize = std::stoi(line);
                            if(mappingSize <= 0 ) return false;
                            break;
                        case 2: //first midi note number to retune
                            rangeToRetune.first = std::stoi(line);
                            break;
                        case 3: //last midi note number to retune
                            rangeToRetune.second = std::stoi(line);
                            break;
                        case 4: // middle note
                            middleNoteFreqPair.first = std::stoi(line);
                            middleNoteFreqPair.second = -1;
                            if(middleNoteFreqPair.first < 0 || middleNoteFreqPair.first > 127) fileReadCorrectly = false;
                            break;
                        case 5: //midi reference note
                            referenceMidiFreqPair.first = std::stoi(line);
                            break;
                        case 6: //frequency of reference midi note
                            referenceMidiFreqPair.second = std::stof(line);
                            if(referenceMidiFreqPair.second <= 0.0) fileReadCorrectly = false;
                            break;
                        case 7: //scale number to consider formal octave.
                            //this value is 1-indexed in the file. We want it to be 0-indexed, so subtract 1.
                            formalOctaveScaleDegree = std::stoi(line) - 1;
                            if(formalOctaveScaleDegree < 0 || formalOctaveScaleDegree >= scaleLength) fileReadCorrectly = false;
                            break;
                        default:
                            if(line.at(0) == 'x') { //in this case, we are mapping 'x' to -1.
                                mapping.push_back(-1);
                            }
                            else{
                                mapping.push_back(std::stoi(line));
                                if(mapping.back() < 0) fileReadCorrectly = false;
                            }
                    }
                    if(!fileReadCorrectly) break; // we don't need to keep reading if there is already an error.
                }
            }
            kbmFile.close();
        }
        catch (...) {fileReadCorrectly = false;} // if formatted incorrectly, return false
        
        
        if(fileReadCorrectly && (lineNum < 7)) fileReadCorrectly = false; // if lineNum < 7, not all meta values were read.
        if(fileReadCorrectly) // we don't need to do this if there is already an error.
        {
            for(size_t i = mapping.size(); i < mappingSize ; i++) {mapping.push_back(-1);}//fill rest of mappings with -1
            if(mapping.size() != mappingSize) fileReadCorrectly = false; //wrong mapping size
            //wrong mapping size
        }
        if(fileReadCorrectly)
        {
            //calcMiddleNoteFreq();
            return true;
        }
        else
        {
            rangeToRetune = beforeRead.retune;
            middleNoteFreqPair = beforeRead.middle;
            referenceMidiFreqPair = beforeRead.ref;
            formalOctaveScaleDegree = beforeRead.octave;
            mapping = beforeRead.mp;
            return false;
        }
    }
    else return false; // if file didn't open, return false;
}
bool KeyboardMap::loadKbmString(std::string kbmString)
{
    std::string tmpfilePath = utils::makeAndWriteTmpFile(kbmString);
    
    bool output = loadKbmFile(tmpfilePath);
    remove(tmpfilePath.c_str());
    return output;
}

/*
 Returns the scale degree for a given midi note
 */
//TODO: fix so that it only works when midiNoteNum is in the correct midiRange
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

//this could be a signed char
int KeyboardMap::getOctave(signed char midiNoteNum)
{
    signed char diff = midiNoteNum -  middleNoteFreqPair.first;
    int output = (int) diff / (int) mapping.size();
    if(diff < 0 && diff % (int) mapping.size() != 0) output--;//because division is symettric around 0, we need to decrement the octave when diff is negative.
    return output;
}
