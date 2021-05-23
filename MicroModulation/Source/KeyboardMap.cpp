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
        int numNotes = -1;
        try
        {
            int mappingSize;
            while(getline(kbmFile, line))
            {
                line = removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    if(++lineNum > 7)
                    {
                        mapping.push_back(std::stoi(line));
                    }
                    else if(lineNum == 1) //then line = the size of map. after how many keys the pattern repeats.
                    {
                        mappingSize = std::stoi(line);
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
                        middleNote = std::stoi(line);
                    }
                    else if(lineNum == 5) //midi reference note
                    {
                        referenceMidiFreqPair.first = std::stoi(line);
                    }
                    else if(lineNum == 6) //frequence of reference midi note
                    {
                        referenceMidiFreqPair.second = std::stof(line);
                    }
                    else if(lineNum == 7) //scale number to consider formal octave.
                    {
                        formalOctaveScaleDegree = std::stoi(line);
                    }

                }
            }
            kbmFile.close();
            
            if(mapping.size() > mappingSize) return false;
            for(size_t i = mapping.size(); i < mappingSize ; i++)
            {
                mapping.push_back(-1);
            }
            
            if(mapping.size() != mappingSize) return false;
            if(numNotes == 0){
                notes.push_back(1.0);
            }
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
