/*
 ==============================================================================
 
 Scale.cpp
 Created: 22 May 2021 12:11:25pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#include <cstdio>
#include <fstream>
#include <string>

#include "JuceHeader.h"

#include "Identifiers.h"
#include "Scale.h"
#include "utils.h"

Scale::Scale(juce::UndoManager& um): scaleValues(IDs::scale), undoManager(um), kbm(um), hasScl(false)
{
    scaleValues.setProperty(IDs::scaleDescription, "", &undoManager);
    scaleValues.setProperty(IDs::scaleNotes, juce::Array<juce::var>(), &undoManager);
    scaleValues.setProperty(IDs::fundamentalFreq, juce::var(440.0f), &undoManager);
    
    scaleValues.addChild(kbm.keyboardMapValues, -1, &undoManager);
    
    initCalculatedFreqs();
}
Scale::Scale(juce::UndoManager& um, std::string sclPath): Scale(um) { loadSclFile(sclPath); }
Scale::Scale(juce::UndoManager& um, std::string sclPath, std::string kbmPath): Scale(um, sclPath) { loadKbmFile(kbmPath); }

bool Scale::loadSclFile(std::string sclPath)
{
    std::ifstream sclFile(sclPath);
    std::string line;
    if(sclFile.is_open())
    {
        undoManager.beginNewTransaction(); //if file read isn't sucessful, will reset to current state.
        bool fileReadCorrectly = true;
        getNotes().clear();

        try{
            int lineNum = 0;
            int numNotesToRead = -1;
            while(getline(sclFile, line))
            {
                //TODO: HACK maybe change implementation so we don't need an extra if block
                if(lineNum == 0 && utils::removeWhiteSpace(line) == ""){//handles files with empty descriptions. hacky.
                    scaleValues.setProperty(IDs::scaleDescription, juce::var(""), &undoManager);
                    lineNum++;
                }
                
                line = utils::removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    switch(++lineNum)
                    {
                        case 1:
                            scaleValues.setProperty(IDs::scaleDescription, juce::var(line), &undoManager);
                            break;
                        case 2:
                            numNotesToRead = std::stoi(line);
                            if(numNotesToRead < 0) fileReadCorrectly = false; //there need to be at least 0 notes!!!
                            break;
                        default:
                            size_t divider;
                            float noteRatio = std::stof(line, &divider);
                            
                            if(line.substr(0,divider+1).find('.') != std::string::npos)//if the value contains a decimal (need +1 so things like "200." will be included.
                            { //then line in decimal/cents format
                                noteRatio = Scale::centsToRatio(noteRatio);
                            }
                            else {
                                line = line.substr(divider); //trimming off the first value
                                utils::removeLineSpaceAndComments(line);  //trimming off any additional white space
                                if(line.size() > 0 && line.at(0) == '/') {//if the next char is a '/', we are in ratio format.
                                    line = line.substr(1); //remove '/' charachter
                                    noteRatio = noteRatio / (float)std::stoi(line);
                                }
                            }
                            if(noteRatio <= 0) fileReadCorrectly = false;
                            getNotes().add(noteRatio);
                            //if(notes.back() <= 0) fileReadCorrectly = false; //notes must be positive.
                    }
                }
                if(!fileReadCorrectly) break; // we don't need to keep reading if there is already an error!
            }
            sclFile.close();
            
            if(fileReadCorrectly && getNotes().size() != numNotesToRead) fileReadCorrectly = false;
            if(fileReadCorrectly && numNotesToRead == 0){
                getNotes().add(juce::var(1.0f));
            }
        }
        catch (...) {fileReadCorrectly = false; } // if formatted incorrectly, return false
        
        if(fileReadCorrectly)
        {
            kbm.setToDefaultMapping(getNotes().size());
            calcFundamentalFreq();
            initCalculatedFreqs();
            hasScl = true;
            return true;
        }
        else // if the file isn't read correctly, return to previous state and return false
        {
            undoManager.undo();
            return false;
        }
    }
    else return false; // if file didn't open, return false;
}


bool Scale::loadSclFile(juce::File sclFile)
{
    return loadSclFile(sclFile.getFullPathName().toStdString());
}
bool Scale::loadSclString(std::string sclString)
{
    std::string tmpfilePath = utils::makeAndWriteTmpFile(sclString);
    
    bool output = loadSclFile(tmpfilePath);
    remove(tmpfilePath.c_str());
    return output;
}

//probably don't actually need these
bool Scale::loadKbmFile(std::string kbmPath)
{
    bool output = kbm.loadKbmFile(kbmPath);
    calcFundamentalFreq();
    return output;
}
bool Scale::loadKbmFile(juce::File kbmFile)
{
    return loadKbmFile(kbmFile.getFullPathName().toStdString());
}
bool Scale::loadKbmString(std::string kbmString)
{
    bool output = kbm.loadKbmString(kbmString);
    calcFundamentalFreq();
    return output;
}


//TODO: test this
/*
 Returns the frequency that should be played back.
 @param midiNoteNum the midiNote number.
 @return the frequncy that is associated with that midi note number.
 */
float Scale::getFreq(juce::int8 midiNoteNum)
{
    assert(midiNoteNum >= 0);
    float calculatedFreq = calculatedFreqs.getUnchecked(midiNoteNum);
    
    if(freqHasBeenCalculated(midiNoteNum)) return calculatedFreq;
    else{
        calculatedFreq = (float) getNote(kbm.getScaleDegree(midiNoteNum-1))
        * pow( (float)getNote(kbm.getFormalOctaveScaleDegree()), kbm.getOctave(midiNoteNum-1))
        * (float) scaleValues.getProperty(IDs::fundamentalFreq);
        
        calculatedFreqs.set(midiNoteNum, calculatedFreq);
        return calculatedFreq;
    }
}
/*
 Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
 @param center midino
 @param pivot
 */
void Scale::modulate(juce::int8 center, juce::int8 pivot)
{
    if(center != pivot) //if center == pivot, modulation does nothing. this can be made more general if optimization is nescicarry
    {
        undoManager.beginNewTransaction();
        initCalculatedFreqs();
    }
}



/*
 Calculates the 'fundamental' or reference frequency for the scale. This corresponds to the frequency that this->kbm.getMiddleNote() is mapped to.
 */
void Scale::calcFundamentalFreq()
{
    scaleValues.setProperty(IDs::fundamentalFreq,
                            kbm.getReferenceFreq() / (float) getNote( kbm.getScaleDegree(kbm.getReferenceMidiNote()) ) //freq of first note in Notes at refernce octave
                            * (float) getNote(kbm.getScaleDegree( kbm.getMiddleNote()) ) //ratio/note corresponding to the middlenote.
                            * pow( (float) getNote(kbm.getFormalOctaveScaleDegree()), -kbm.getOctave(kbm.getReferenceMidiNote())), //octave multiplier to get to the middle note from the reference note
                            &undoManager);
}



void Scale::initCalculatedFreqs()
{
    calculatedFreqs.clearQuick();
    calculatedFreqs.resize(128); //128, because there are 128 midi values
    calculatedFreqs.fill(-1.0f);
}

bool Scale::freqHasBeenCalculated(juce::int8 midiNoteNum)
{
    assert(midiNoteNum >= 0);
    return calculatedFreqs.getUnchecked(midiNoteNum) != -1.0;
}
