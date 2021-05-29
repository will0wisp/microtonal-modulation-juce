#pragma once


#include <string>

#include "JuceHeader.h"

#include "Scale.h"
#include "KeyboardMap.h"


class MidiProcessor
{
public:
    MidiProcessor(){}
    /**
     The main process block for Midi messages. Using a .scl file, it retunes the message using MPE and pitchbend.
     */
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
    
    float getFreq(char midiNote)
    {
        assert(midiNote >= 0);
        assert(midiNote <= 127);
        return -1;
    }
    
    juce::MidiBuffer processedBuffer;
    Scale scale;
    KeyboardMap kbMap;
};
