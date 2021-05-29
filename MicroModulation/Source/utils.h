/*
 ==============================================================================
 
 utils.h
 Created: 26 May 2021 11:24:07am
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <regex>


namespace utils {

/*
 Helper function for Scale::loadSclString() (and KeyboardMap::loadKbmString() ).
 @param contents The contents to be written to the file.
 note: cstdio::remove() must be called on this file!!!!
 @return path to the tmp file.
 */
static std::string makeAndWriteTmpFile(std::string contents)
{
    //Make temporary file path
    char tmpfilePath[80]= "/tmp/microMod.XXXXXX"; //don't know what 80 size is about. took it from stackexchange
    mkstemp(tmpfilePath);
    //write contents to tmpFile
    FILE* tmpFile = fopen(tmpfilePath, "w");
    fputs(contents.c_str(), tmpFile);
    fclose(tmpFile);
    
    return tmpfilePath;
    
}


/*
 Helper function for file reading.
 Removes leading white space and comments from a .scl or .kbm file line.
 */
static std::string removeLineSpaceAndComments(std::string line)
{
    std::string output = std::regex_replace(line, std::regex("^\\s+"), std::string("")); //remove initial whitespace
    output = std::regex_replace(output, std::regex("!.*"), std::string("")); //remove inline comments
    //TODO: test following line. i'm not sure if this actually works, and is a hack solution to a bug.
    output = std::regex_replace(output, std::regex("\r$"), std::string("")); //remove lone /r .
    return output;
}

static std::string makeKbmString(int numNotes,
                                 int mapLowBound, int mapHighBound,
                                 int middleNote,
                                 int refNote, float refFreq,
                                 int formalOctaveScaleDegree,
                                 std::vector<int> mappings)
{
    std::string output = std::to_string(numNotes) + "\n"
    + std::to_string(mapLowBound) + "\n" + std::to_string(mapHighBound) + "\n"
    + std::to_string(middleNote) + "\n"
    + std::to_string(refNote) + "\n" + std::to_string(refFreq) + "\n"
    + std::to_string(formalOctaveScaleDegree) + "\n";
    for(int i : mappings)
    {
        output = output + std::to_string(i) + "\n";
    }
    return output;
}

static std::string makeKbmString(int numNotes,
                                 int mapLowBound, int mapHighBound,
                                 int middleNote,
                                 int refNote, float refFreq,
                                 int formalOctaveScaleDegree,
                                 std::vector<std::string> mappings)
{
    std::string output = std::to_string(numNotes) + "\n"
    + std::to_string(mapLowBound) + "\n" + std::to_string(mapHighBound) + "\n"
    + std::to_string(middleNote) + "\n"
    + std::to_string(refNote) + "\n" + std::to_string(refFreq) + "\n"
    + std::to_string(formalOctaveScaleDegree) + "\n";
    for(std::string s : mappings)
    {
        output = output + s + "\n";
    }
    return output;
}


static std::string makeSclString(std::string description, std::string numNotes, std::vector<std::string> notes)
{
    std::string output = description + "\n" + numNotes + "\n";
    for(std::string n : notes)
    {
        output = output + n + "\n";
    }
    return output;
}


static double getMidiNoteInHertz (const int noteNumber, const double frequencyOfA)
{
    return frequencyOfA * std::pow (2.0, (noteNumber - 69) / 12.0);
}

}  // end namespace utils
