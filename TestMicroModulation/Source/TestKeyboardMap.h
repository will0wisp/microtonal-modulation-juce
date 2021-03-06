/*
 ==============================================================================
 
 TestReadKbm.h
 Created: 23 May 2021 5:07:52pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <stdio.h>

#include "Catch/catch_amalgamated.hpp"

#include "../../MicroModulation/Source/KeyboardMap.h"
#include "../../MicroModulation/Source/utils.h"

//TODO: test that the kbm doesn't get changed if the load is failed
TEST_CASE("Test KeyboardMap::loadKbmFile")
{
    juce::UndoManager um;
    KeyboardMap kb(um);
    SECTION("Test meta values read correctly.")
    {
        KeyboardMap kb(um, 7);
        kb.loadKbmString(utils::makeKbmString(1,
                                       2,3,
                                       4,
                                       5,6.0,
                                       7,
                                       {1}));
        SECTION("Test range to remap")
        {
            REQUIRE(kb.getRetuneRangeLowerBound() == 2);
            REQUIRE(kb.getRetuneRangeUpperBound() == 3);
        }
        SECTION("Test middle note read correctly")
        {
            REQUIRE(kb.getMiddleNote() == 4);
        }
        SECTION("Test midi reference note and freq read correctly")
        {
            REQUIRE(kb.getReferenceMidiNote() == 5);
            REQUIRE(kb.getReferenceFreq() == 6.0);
        }
        //note formal octave is 1-indexed in the file. in .getFormalOctaveScaleDegree, it is 0-indexed. this is why we add 1.
        SECTION("Test formal octave read correctly")
        {
            REQUIRE(kb.getFormalOctaveScaleDegree() + 1 == 7);
        }
    }
    SECTION("Test bad meta values.")
    {
        KeyboardMap kb(um);
        kb.loadKbmFile(utils::makeKbmString(2,
                                            1, 127,
                                            45,
                                            47, 330.0,
                                            2,
                                            {1, 2}));
        
        juce::ValueTree before = kb.keyboardMapValues.createCopy();
        
        SECTION("Bad mapping size.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(-1, //out of range
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         7,
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));
        }
        SECTION("Bad middle note.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         -1, //out of range
                                                         5,6.0,
                                                         1,
                                                         {1})));
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         128, //out of range
                                                         5,6.0,
                                                         1,
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));
        }
        SECTION("Bad frequency reference.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,-1.0,//out of range
                                                         1,
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));

            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,0.0,//out of range
                                                         1,
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));
        }
        SECTION("Bad formal octave scale degree")
        {
            KeyboardMap kb(um, 10);
            juce::ValueTree before = kb.keyboardMapValues.createCopy();
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         11,//out of range
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));

            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         0,//out of range
                                                         {1})));
            REQUIRE(kb.keyboardMapValues.isEquivalentTo(before));
        }
        
    }
    SECTION("Test mappings read correctly")
    {//TODO: something is fishy in this test. If we delete some of the notes in the .scl string, it still passes, even though it shouldn't.
        KeyboardMap kb(um, 14);
        kb.loadKbmString(utils::makeKbmString(5,
                                       2,3,
                                       4,
                                       5,6.0,
                                       7,//out of range
                                       {10,11,12,13,14}));
        for(int i = 0; i < 5; i++)
        {
            REQUIRE(kb.getMapping(i) == i + 10); //the i+10 is just because  I set the file up so this loop would work.
            REQUIRE(kb.getMapping().size() == 5);
        }
    }
    SECTION("Test too many mappings.")
    {
        REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(6,
                                                     0,127,
                                                     60,
                                                     69,440.0,
                                                     1,
                                                     {7,6,5,4,3,2,1})));//7 mappings. should only be 6 here.

    }
    SECTION("Test mappings left out read as -1.")
    {
        KeyboardMap kb(um, 6);
        REQUIRE(kb.loadKbmString(utils::makeKbmString(6,
                                               0,127,
                                               60,
                                               69,440.0,
                                               1,
                                               (std::vector<int>){})));
        int mappingSize = 6;
        REQUIRE(kb.getMapping().size() == 6);
        for(int i = 0; i < mappingSize; i++)
        {
            REQUIRE(kb.getMapping(i) == -1);
        }
    }
    
    SECTION("Test 'x' mappings read as -1.")
    {
        KeyboardMap kb(um, 6);
        REQUIRE(kb.loadKbmString(utils::makeKbmString(6,
                                               0,127,
                                               60,
                                               69,440.0,
                                               1,
                                               {"x","1","x","x", "1", "x"})));
        int mappingSize = 6;
        REQUIRE(kb.getMapping().size() == 6);
        for(int i = 0; i < mappingSize; i++)
        {
            if(i == 1 || i == 4){
                REQUIRE(kb.getMapping(i) == 1);
            }
            else {
                REQUIRE(kb.getMapping(i) == -1);
            }
        }
    }
    
}


TEST_CASE("Test KeyboardMap::getScaleDegree")
{
    juce::UndoManager um;
    KeyboardMap kb(um, 6);
    int numTests = 50;
    std::srand(time(NULL));

    
    int numNotes = 6;
    int mapLowBound = 0; int mapHighBound = 127;
    int middleNote = 60;
    int refNote = 69; float refFreq= 440.;
    int formalOctaveScaleDegree=0;
    std::vector<int> mappings={1,2,3,4,5,6};

    for(int test = 0; test < numTests; test++)
    {
        middleNote = utils::randInt(0,128);
//        mapLowBound = rand() % 127;
//        mapHighBound = mapLowBound + rand() % (127 - mapLowBound);
        refNote = utils::randInt(0,128);
        formalOctaveScaleDegree = utils::randInt(0,5) + 1; // + 1, because is 1-indexed in file
        
        assert(kb.loadKbmString(utils::makeKbmString(numNotes, mapLowBound, mapHighBound, middleNote, refNote, refFreq, formalOctaveScaleDegree, mappings)));
        REQUIRE(kb.getScaleDegree(middleNote) == 1); //tests middleNote is the first mapped note
        
        //this for loop tests the mapping of every other note
        for(int offset = 0; offset < numNotes; offset++ ){//'offset+1' should map to mappings.at(i)
            for(int i = middleNote + offset; i < 128; i += numNotes)
            {
                REQUIRE(kb.getScaleDegree(i) == offset+1);//midiNote >= middleNote
            }
            for(int i = middleNote + offset; i >= 0 && i <128; i -= numNotes)
            {
                REQUIRE(kb.getScaleDegree(i) == offset+1);//midiNote < middleNote
            }
        }
    }
}
TEST_CASE("Test KeyboardMap::getOctave")
{
    juce::UndoManager um;
    KeyboardMap kb(um, 6);
    int numTests = 50;
    
    int numNotes = 6;
    int mapLowBound = 0; int mapHighBound = 127;
    int middleNote = 60;
    int refNote = 69; float refFreq= 440.;
    int formalOctaveScaleDegree=6;
    std::vector<int> mappings={1,2,3,4,5,6};
     
    for(int test = 1; test <= numTests; test++){
        assert(kb.loadKbmString(utils::makeKbmString(numNotes, mapLowBound, mapHighBound, middleNote, refNote, refFreq, formalOctaveScaleDegree, mappings)));
        
        int octave = 0;
        for(int i = middleNote; i < 128; i+= numNotes)
        {
            for(int j = 0; j < numNotes && i+j < 128; j++)
            {
                REQUIRE(kb.getOctave(i+j) == octave);
            }
            octave++;
        }
        octave = 0;
        for(int i = middleNote; i>=0 && i < 128; i-= numNotes)
        {
            for(int j = 0; j < numNotes && i+j < 128; j++)
            {
                REQUIRE(kb.getOctave(i+j) == octave);
            }
            octave--;
        }
    }

}
