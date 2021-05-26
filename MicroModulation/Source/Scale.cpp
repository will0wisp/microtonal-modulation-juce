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
        int lineNum = 0;
        int numNotes = -1;
        try
        {
            while(getline(sclFile, line))
            {
                line = utils::removeLineSpaceAndComments(line);
                if(line != "") //if our line wasn't just a comment line
                {
                    if(++lineNum > 2)
                    {
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
                        if(notes.back() <= 0) return false; //notes must be positive.
                    }
                    else if(lineNum == 1)
                    {
                        description = line;
                    }
                    else if(lineNum == 2)
                    {
                        numNotes = std::stoi(line);
                        if(numNotes < 0) return false; //there need to be at least 0 notes!!!
                    }
                }
            }
            sclFile.close();
            
            if(notes.size() != numNotes) return false;
            if(numNotes == 0){
                notes.push_back(1.0);
            }
        }
        catch (...) { return false; } // if formatted incorrectly, return false
        return true;
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
