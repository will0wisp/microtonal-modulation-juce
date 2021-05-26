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

#include "Scale.h"
#include "utils.h"

Scale::Scale(){
    
}

bool Scale::loadSclFile(std::string sclPath)
{
    std::ifstream sclFile(sclPath);
    std::string line;
    if(sclFile.is_open())
    {
        StoredScale beforeRead = StoredScale(*this);
        bool fileReadCorrectly = true;
        
        int lineNum = 0;
        int numNotes = -1;
        
        
        try
        {
            while(getline(sclFile, line))
            {
                line = utils::removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    switch(++lineNum)
                    {
                        case 1:
                            description = line;
                            break;
                        case 2:
                            numNotes = std::stoi(line);
                            if(numNotes < 0) fileReadCorrectly = false; //there need to be at least 0 notes!!!
                            break;
                        default:
                            if (line.find('.') != std::string::npos)
                            { //line in decimal/cents format
                                notes.push_back( Scale::centsToRatio(std::stof(line)) );
                            }
                            else if (line.find('/') != std::string::npos)
                            { //line in ratio/fraction format
                                size_t divider; // position of '/' in the line
                                notes.push_back(std::stof(line, &divider) / std::stof(line.substr(divider+1)));
                            }
                            else
                            {
                                notes.push_back(std::stoi(line));
                            }
                            if(notes.back() <= 0) fileReadCorrectly = false; //notes must be positive.
                    }
                }
                if(!fileReadCorrectly) break; // we don't need to keep reading if there is already an error!
            }
            sclFile.close();
            
            if(notes.size() != numNotes) fileReadCorrectly = false;
            if(numNotes == 0){
                notes.push_back(1.0);
            }
        }
        catch (...) {fileReadCorrectly = false; } // if formatted incorrectly, return false
        
        if(fileReadCorrectly)
        {
            kbm = KeyboardMap((int)notes.size());
            return true;
        }
        else // if the file isn't read correctly, return to previous state and return false
        {
            description = beforeRead.d;
            notes = beforeRead.n;
            kbm = beforeRead.kbm;
            return false;
        }
    }
    else return false; // if file didn't open, return false;
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
    return kbm.loadKbmFile(kbmPath);
}
bool Scale::loadKbmString(std::string kbmString)
{
    return kbm.loadKbmString(kbmString);
}


//
///*
// Returns the frequency that should be played back.
// @param midiNoteNum the midiNote number.
// @return the frequncy that is associated with that midi note number.
// */
//float Scale::getFreq(signed char midiNoteNum)
//{
//    kbm.getScaleDegree(signed char midiNoteNum)
//}
///*
// Modulates from center to pivot. The frequency-ratios around pivot after modulation will be the same as those around center before modulation.
// @param center midino
// @param pivot
// */
//void Scale::modulate(signed char center, signed char pivot)
//{
//    
//}
