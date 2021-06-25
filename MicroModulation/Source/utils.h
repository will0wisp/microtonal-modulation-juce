/*
 ==============================================================================
 A namespace of static, miscellaneous, utility functions.
 These are mostly helper functions for file parsing that are useful for testing.
 
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

/**
 Helper function for Scale::loadSclString() (and KeyboardMap::loadKbmString() ).
 @param contents. The string to be written to a new tmp file.
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


/**
 Helper function for file reading.
 Removes leading white space and comments (signified by '!') from a .scl or .kbm file line.
 @param line. The string to remove spaces and comments from.
 */
static std::string removeLineSpaceAndComments(std::string line)
{
    std::string output = std::regex_replace(line, std::regex("^\\s+"), std::string("")); //remove initial whitespace
    output = std::regex_replace(output, std::regex("!.*"), std::string("")); //remove inline comments
    //TODO: test following line. i'm not sure if this actually works, and is a hack solution to a bug.
    output = std::regex_replace(output, std::regex("\r$"), std::string("")); //remove lone /r .
    return output;
}
/**
 Removes initial white space and handels possible '/r/n' line breaks from a line in a .scl or .kbm file.
 @param line. The string to remove white space from.
 @return The string resluting after white space has been removed from line.
 */
static std::string removeWhiteSpace(std::string line)
{
    std::string output = std::regex_replace(line, std::regex("\s+"), std::string("")); //remove initial whitespace
    output = std::regex_replace(output, std::regex("\r$"), std::string("")); //remove lone /r .
    return output;
}



/**
 Makes a string formatted as a .kbm file to use with utils::makeAndWriteTmpFile to load custom Keyboard Maps.
 @param sizeOfMap After how many notes the mapping repeats.
 @param mapLowBound The first midi note value to retune.
 @param mapHighBound The last midi note value to retune.
 @param middleNote The note value to which the first entry of the mapping should be mapped.
 @param refNote Reference note for which frequency is given.
 @param refFreq Frequency to tune the above reference note to.
 @param formalOctaveScaleDegree The Scale degree to consider as a formal octave. It determines the difference in pitch between adjacent mapping patterns.
 @param mappings A std::vector<std::string>> that stores which keys should be mapped to which scale degrees.
 @return A string that is formatted as a .kbm file based on the given parameters.
 */
static std::string makeKbmString(int sizeOfMap,
                                 int mapLowBound, int mapHighBound,
                                 int middleNote,
                                 int refNote, float refFreq,
                                 int formalOctaveScaleDegree,
                                 std::vector<std::string> mappings)
{
    std::string output = std::to_string(sizeOfMap) + "\n"
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

static std::string makeKbmString(int sizeOfMap,
                                 int mapLowBound, int mapHighBound,
                                 int middleNote,
                                 int refNote, float refFreq,
                                 int formalOctaveScaleDegree,
                                 std::vector<int> mappings)
{
    std::vector<std::string> strMappings = std::vector<std::string>();
    for(int m : mappings) strMappings.push_back(std::to_string(m));
    return makeKbmString(sizeOfMap,
                         mapLowBound, mapHighBound,
                         middleNote,
                         refNote, refFreq,
                         formalOctaveScaleDegree,
                         strMappings);
}


/**
 Makes a string formatted as a .scl file to use with utils::makeAndWriteTmpFile to load custom Scales (tunings)
 @param description A string that describes the scale/tuning.
 @param numNotes The number of notes in the scale.
 @param notes std::vector<std::string>> of ratios/intervals that describe the notes in the scale/tuning.
 @return A string that is formatted as a .scl file based on the given parameters.
 */
static std::string makeSclString(std::string description, std::string numNotes, std::vector<std::string> notes)
{
    std::string output = description + "\n" + numNotes + "\n";
    for(std::string n : notes)
    {
        output = output + n + "\n";
    }
    return output;
}
static std::string makeSclString(std::string description, std::string numNotes, std::vector<float> notes)
{
    std::string output = description + "\n" + numNotes + "\n";
    for(float n : notes)
    {
        output = output + std::to_string(n) + "\n";
    }
    return output;
}
/**
 Makes a string formatted as a .scl file to use with utils::makeAndWriteTmpFile to load custom Scales (tunings)
 @param description A string that describes the scale/tuning.
 @param numNotes The number of notes in the scale.
 @param notes std::vector<std::float>> of RATIOS that describe the notes in the scale/tuning.
 @return A string that is formatted as a .scl file based on the given parameters.
 */
static std::string makeSclStringWithRatios(std::string description, std::string numNotes, std::vector<float> notes)
{
    std::vector<std::string> strNotes = std::vector<std::string>();
    for(float n : notes) strNotes.push_back(std::to_string(n));
    return makeSclString(description, numNotes, strNotes);
}

//Depreciated. Juce has this function built in at juce::MidiMessages::getMidiNoteInHerz;
static double getMidiNoteInHertz (const int noteNumber, const double frequencyOfA)
{
    return frequencyOfA * std::pow (2.0, (noteNumber - 69) / 12.0);
}


static double freqToMidi(const double freqInHertz, const double frequencyOfA)
{
    return log2(freqInHertz / frequencyOfA) * 12.0 + 69.0;
}


//function from https://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain/20924698#20924698
inline static int mod(int a, int n)
{
    if (n == 0)
        throw std::out_of_range("(a mod 0) is undefined for any a.");

    //puts a in the [-n+1, n-1] range using the remainder operator
    int remainder = a%n;

    //if the remainder is less than zero, add n to put it in the [0, n-1] range if n is positive
    //if the remainder is greater than zero, add n to put it in the [n-1, 0] range if n is negative
    if ((n > 0 && remainder < 0) ||
        (n < 0 && remainder > 0))
        return remainder + n;
    return remainder;
}

static int randInt(int low, int high)
{
    if(low > high) return randInt(high, low);
    return rand() % (high - low) + low;
}

static float randFloat(float low, float high)
{
    if(low > high) return randInt(high, low);
    return low + static_cast<float> (rand()) / static_cast <float> (RAND_MAX) * (high - low);
}


}  // end namespace utils


