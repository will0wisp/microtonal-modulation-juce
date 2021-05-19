#pragma once

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
    
    juce::MidiBuffer processedBuffer;
};


#if JUCE_UNIT_TESTS
class ModulateTest  : public UnitTest
{
public:
    ModulateTest()  : UnitTest ("Foobar testing") {}
 
    void runTest() override
    {
        beginTest ("Part 1");
 
        expect (myFoobar.doesSomething());
        expect (myFoobar.doesSomethingElse());
 
        beginTest ("Part 2");
 
        expect (myOtherFoobar.doesSomething());
        expect (myOtherFoobar.doesSomethingElse());
 
        ...etc..
    }
};
 
// Creating a static instance will automatically add the instance to the array
// returned by UnitTest::getAllTests(), so the test will be included when you call
// UnitTestRunner::runAllTests()
static MyTest test;
#endif
