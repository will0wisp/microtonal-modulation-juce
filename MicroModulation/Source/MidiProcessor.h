#pragma once

#include <cmath>
#include <fstream>
#include <string>

#include "JuceHeader.h"


class MidiProcessor
{
public:
    void process(juce::MidiBuffer& midiMessages)
    {
        juce::MidiMessage message;
        for(const juce::MidiMessageMetadata metadata : midiMessages)
        {

            message = metadata.getMessage();
            if(message.isNoteOnOrOff()){
                message.setNoteNumber(message.getNoteNumber()+4);
            }
            processedBuffer.addEvent(message, metadata.samplePosition);
        }
        midiMessages.swapWith(processedBuffer);
    }
    
    /**
     Reads in a .scl file.
     @param sclPath the path of the file.
     @return true if file is read successfully. false otherwise
     */
    bool loadSclFile(std::string sclPath){
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
                    if(line.at(0) != '!') // lines starting with '!' are comments
                    {
                        //removes terminal inline comment, if there is one.
                        size_t commentStart = line.find('!');
                        if(commentStart != std::string::npos)
                        {
                            line = line.substr(0, commentStart);
                        }
                        
                        
                        if(++lineNum > 2)
                        {
                            if (line.find('.') != std::string::npos)
                            { //line in decimal/cents format
                                scale.notes.push_back( Scale::centsToRatio(std::stof(line)) );
                            }
                            else if (line.find('/') != std::string::npos)
                            { //line in ratio/fraction format
                                size_t divider; // position of '/' in the line
                                scale.notes.push_back(std::stof(line, &divider) / std::stof(line.substr(divider+1)));
                            }
                            else
                            {
                                scale.notes.push_back(std::stoi(line));
                            }
                            if(scale.notes.back() <= 0) return false; //notes must be positive.
                        }
                        else if(lineNum == 1)
                        {
                            scale.description = line;
                        }
                        else if(lineNum == 2) 
                        {
                            numNotes = std::stoi(line);
                            if(numNotes < 0) return false; //there need to be at least 0 notes!!!
                        }
                    }
                }
                sclFile.close();
                
                if(scale.notes.size() != numNotes) return false;
                if(numNotes == 0){
                    scale.notes.push_back(1.0);
                }
            }
            catch (...) { return false; } // if formatted incorrectly, return false
            return true;
        }
        else return false; // if file didn't open, return false;
    }
    
    
    float getFreq(char midiNote)
    {
        assert(midiNote >= 0);
        assert(midiNote <= 127);
        
        
        return -1;
    }
    
    
    struct Scale{
        std::string description;
        std::vector<float> notes;
        
        static float centsToRatio(float cents){
            return std::pow(M_E, (cents * std::log(2.0) / 1200) );
        }
    };
    
    juce::MidiBuffer processedBuffer;
    Scale scale;
};
