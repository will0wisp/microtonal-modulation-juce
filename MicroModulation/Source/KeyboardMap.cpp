/*
 ==============================================================================
 
 KeyboardMap.cpp
 Created: 23 May 2021 5:06:31pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#include <cstdio>
#include <fstream>

#include "JuceHeader.h"

#include "Identifiers.h"  //stores all juce::Identifier s in namespace "IDs"
#include "KeyboardMap.h"
#include "utils.h"

KeyboardMap::KeyboardMap(juce::UndoManager& um) : keyboardMapValues(IDs::keyboardMap), undoManager(um)
{
    setDefaultValues();
}

KeyboardMap::KeyboardMap(juce::UndoManager& um, int sclLength) : KeyboardMap(um) { setToDefaultMapping(sclLength); }
KeyboardMap::KeyboardMap(juce::UndoManager& um, int sclLength, std::string kbmPath) : KeyboardMap(um)
{
    keyboardMapValues.setProperty(IDs::scaleLength, sclLength, &undoManager);
    loadKbmFile(kbmPath);
}

void KeyboardMap::setDefaultValues()
{
    keyboardMapValues.setProperty(IDs::retuneRangeLowerBound,
                                  (signed char) 0, &undoManager);
    keyboardMapValues.setProperty(IDs::retuneRangeUpperBound,
                                  (signed char) 127, &undoManager);
    keyboardMapValues.setProperty(IDs::middleNote,
                                  (signed char) 60, &undoManager);
    keyboardMapValues.setProperty(IDs::referenceNote,
                                  (signed char) 69, &undoManager);
    keyboardMapValues.setProperty(IDs::referenceFreq,
                                  440.f, &undoManager);
    keyboardMapValues.setProperty(IDs::formalOctaveScaleDegree,
                                  -1, &undoManager);
    keyboardMapValues.setProperty(IDs::scaleLength,
                                  -1, &undoManager);
    keyboardMapValues.setProperty(IDs::keyboardMapping,
                                  juce::Array<juce::var>(), &undoManager);
}

void KeyboardMap::setToDefaultMapping(int scaleLength)
{
    setDefaultValues();
    keyboardMapValues.setProperty(IDs::scaleLength,
                                  scaleLength, &undoManager);
    keyboardMapValues.setProperty(IDs::formalOctaveScaleDegree,
                                  scaleLength - 1, &undoManager);
    for(int i = 0; i < scaleLength; i++)
    {
        getMapping().add(juce::var(i));
    }
}

bool KeyboardMap::loadKbmFile(std::string kbmPath)
{
    std::ifstream kbmFile(kbmPath);
    std::string line;
    if(kbmFile.is_open())
    {
        undoManager.beginNewTransaction();
        bool fileReadCorrectly = true;
        getMapping().clear();
        
        try
        {
            int lineNum = 0;
            int mappingSize;
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
                            keyboardMapValues.setProperty(IDs::retuneRangeLowerBound, std::stoi(line), &undoManager);
                            break;
                        case 3: //last midi note number to retune
                            keyboardMapValues.setProperty(IDs::retuneRangeUpperBound, std::stoi(line), &undoManager);
                            break;
                        case 4: // middle note
                        {
                            int middleNote = std::stoi(line);
                            keyboardMapValues.setProperty(IDs::middleNote, middleNote, &undoManager);
                            if(middleNote < 0 || middleNote > 127) fileReadCorrectly = false;
                            break;
                        }
                        case 5: //midi reference note
                            keyboardMapValues.setProperty(IDs::referenceNote, std::stoi(line), &undoManager);
                            break;
                        case 6: //frequency of reference midi note
                        {
                            float refFreq = std::stof(line);
                            keyboardMapValues.setProperty(IDs::referenceFreq, refFreq, &undoManager);
                            if(refFreq <= 0.0) fileReadCorrectly = false;
                            break;
                        }
                        case 7: //scale number to consider formal octave.
                        {
                            //this value is 1-indexed in the file. We want it to be 0-indexed, so subtract 1.
                            int octaveScaleDegree = std::stoi(line) - 1;
                            keyboardMapValues.setProperty(IDs::formalOctaveScaleDegree, octaveScaleDegree, &undoManager);
                            if(octaveScaleDegree < 0 || octaveScaleDegree >= static_cast<int>(keyboardMapValues.getProperty(IDs::scaleLength))) fileReadCorrectly = false;
                            break;
                        }
                        default:
                            if(line.at(0) == 'x') { //in this case, we are mapping 'x' to -1.
                                getMapping().add(-1);
                            }
                            else{
                                int mappingVal = std::stoi(line);
                                getMapping().add(mappingVal);
                                if(mappingVal < 0) fileReadCorrectly = false;
                            }
                    }
                    if(!fileReadCorrectly) break; // we don't need to keep reading if there is already an error.
                }
            }
            kbmFile.close();
            
            
            if(fileReadCorrectly && (lineNum < 7)) fileReadCorrectly = false; // if lineNum < 7, not all meta values were read.
            if(fileReadCorrectly) // we don't need to do this if there is already an error.
            {
                for(size_t i = getMapping().size(); i < mappingSize ; i++) {getMapping().add(-1);}//fill rest of the mappings with -1
                if(getMapping().size() != mappingSize) fileReadCorrectly = false; //wrong mapping size
            }
        }
        catch (...) {fileReadCorrectly = false;} // if formatted incorrectly, return false
        
        if(fileReadCorrectly)
        {
            return true;
        }
        else
        {
            undoManager.undo();
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

int KeyboardMap::getMappingIndex(juce::int8 midiNoteNum)
{
    assert(getMapping().size() > 0);
    int index;
    if(midiNoteNum >= getMiddleNote())
    {
        index = utils::mod(midiNoteNum - getMiddleNote(), getMapping().size());
    }
    else{
        index = getMapping().size() - utils::mod((getMiddleNote() - midiNoteNum), getMapping().size());
        if(index == getMapping().size()) index = 0;
    }
    return index;
}
/**
 Returns the scale degree for a given midi note
 */
//TODO: fix so that it only works when midiNoteNum is in the correct midiRange
int KeyboardMap::getScaleDegree(juce::int8 midiNoteNum)
{
    return getMapping(getMappingIndex(midiNoteNum));
}

int KeyboardMap::getOctave(juce::int8 midiNoteNum)
{
   juce::int8 diff = midiNoteNum -  getMiddleNote();
    int output = (int) diff / (int) getMapping().size();
    if(diff < 0 && diff % (int) getMapping().size() != 0) output--;//because division is symettric around 0, we need to decrement the octave when diff is negative.
    return output;
}
