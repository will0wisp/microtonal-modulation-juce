#pragma once


#include <string>

#include "JuceHeader.h"

#include "Scale.h"
#include "KeyboardMap.h"
#include "utils.h"

class MidiProcessor
{
private:
    bool hasSentSetupMessages;
    juce::MidiBuffer setupMessages;
    
    juce::MPEZoneLayout zoneLayout; //for channelAssigner
    juce::MPEChannelAssigner channelAssigner;
    
    juce::Array<juce::int8> midiNoteChannelMap; // midiNoteChannelMap[noteNum] stores which channel noteNum is being played on, or -1 if noteNum is not currently mapped/being played
        
    void sendSetupMessages() {
        processedBuffer.addEvents(setupMessages, 0, -1, 0);
        hasSentSetupMessages = true;
    }
    
    void makeSetupMessages() {
        zoneLayout.setLowerZone(15, 1, 2);
        zoneLayout.setUpperZone(0, 1, 2);
        setupMessages.addEvents(juce::MPEMessages::setZoneLayout(zoneLayout),
                                0, -1, 0);
    }
    void initMidiNoteChannelMap() {
        midiNoteChannelMap.resize(128); //128, because there are 128 midi values
        midiNoteChannelMap.fill(static_cast<juce::int8>(-1)); //nothing is currently being played
    }
public:
    MidiProcessor(juce::UndoManager& um) : hasSentSetupMessages(false), undoManager(um), scale(um), midiProcessorValues(juce::Identifier("midiProcessor"))
    {
        makeSetupMessages();
        initMidiNoteChannelMap();
        
        midiProcessorValues.addChild(scale.scaleValues, -1, &undoManager);
        
        midiProcessorValues.setProperty(juce::Identifier("lastNotePlayed"), -1, &undoManager);
    }
    /**
     Function for processing Midi messages. Using a .scl file, it retunes the message using MPE and pitchbend.
     @param midiMessages The MIDI buffer sent from  PluginProcessor::processBlock. Contains all MIDI for processing.
     */
    void process(juce::MidiBuffer& midiMessages)
    {
        processedBuffer.clear();
        if(!hasSentSetupMessages) sendSetupMessages();
        
        if(scale.hasSclLoaded()) //if no scl has been loaded, skip all processing
        {
            juce::MidiMessage message;
            for(const juce::MidiMessageMetadata metadata : midiMessages)
            {
                message = metadata.getMessage();
                
                if(message.isNoteOn())
                {
                    midiProcessorValues.setProperty(juce::Identifier("lastNotePlayed"), message.getNoteNumber(), &undoManager);
                }
                if(message.isNoteOff()) channelAssigner.noteOff(message.getNoteNumber());
                if(message.isAllNotesOff()) channelAssigner.allNotesOff();
                
                if(message.isNoteOnOrOff() || message.isAftertouch()){
                    juce::int8 channel = midiNoteChannelMap.getUnchecked(message.getNoteNumber());
                    if(channel == -1)
                    {
                        channel = channelAssigner.findMidiChannelForNewNote(message.getNoteNumber());
                        midiNoteChannelMap.set(message.getNoteNumber(), channel);
                    }
                    message.setChannel(channel);
                    
                    double unRoundedMidiNoteNum = utils::freqToMidi(scale.getFreq(message.getNoteNumber()), 440.0f);
                    double midiNoteNum = std::round(unRoundedMidiNoteNum);
                    message.setNoteNumber(midiNoteNum);
                    
                    
                    typedef juce::MidiMessage Msg;
                    auto pitchBendVal = Msg::pitchbendToPitchwheelPos(midiNoteNum - unRoundedMidiNoteNum,
                                                                                                     zoneLayout.getLowerZone().perNotePitchbendRange);
                    //DBG(std::to_string(unRoundedMidiNoteNum)+ + " "+ std::to_string(pitchBendVal));
                    processedBuffer.addEvent(Msg::pitchWheel(channel, pitchBendVal), metadata.samplePosition);
                }
                
                if(!message.isPitchWheel()) // currently no handeling for pitch bend messages.
                {
                    processedBuffer.addEvent(message, metadata.samplePosition);
                }
            }
        }
        
        midiMessages.clear();
        midiMessages.swapWith(processedBuffer);
    }
    
    juce::MidiBuffer processedBuffer;
    juce::UndoManager& undoManager;
    Scale scale;
    
    juce::ValueTree midiProcessorValues;
};
