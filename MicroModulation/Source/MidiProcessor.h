#pragma once

#include <cmath>
#include <fstream>
#include <string>
#include <regex>

#include "JuceHeader.h"

#include "Scale.h"


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
        return scale.loadSclFile(sclPath);
    }
    
    
    float getFreq(char midiNote)
    {
        assert(midiNote >= 0);
        assert(midiNote <= 127);
        
        
        return -1;
    }
    
    juce::MidiBuffer processedBuffer;
    Scale scale;
};
