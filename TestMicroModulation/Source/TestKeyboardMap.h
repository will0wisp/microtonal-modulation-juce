/*
 ==============================================================================
 
 TestReadKbm.h
 Created: 23 May 2021 5:07:52pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include <stdio.h>

#include "../../MicroModulation/Source/KeyboardMap.h"
#include "../../MicroModulation/Source/utils.h"

//TODO: test that the kbm doesn't get changed if the load is failed
TEST_CASE("Test KeyboardMap::loadKbmFile")
{
    
    KeyboardMap kb;
    SECTION("Test meta values read correctly.")
    {
        kb.loadKbmString(utils::makeKbmString(1,
                                       2,3,
                                       4,
                                       5,6.0,
                                       7,
                                       {1}));
        SECTION("Test range to remap")
        {
            REQUIRE(kb.getRangeToRetune().first == 2);
            REQUIRE(kb.getRangeToRetune().second == 3);
        }
        SECTION("Test middle note read correctly")
        {
            REQUIRE(kb.getMiddleNoteFreqPair().first == 4);
        }
        SECTION("Test midi reference note and freq read correctly")
        {
            REQUIRE(kb.getRefererenceMidiFreqPair().first == 5);
            REQUIRE(kb.getRefererenceMidiFreqPair().second == 6.0);
        }
        SECTION("Test formal octave read correctly")
        {
            REQUIRE(kb.getFormalOctaveScaleDegree() == 7);
        }
    }
    SECTION("Test bad meta values.")
    {
        KeyboardMap kb;

        kb.loadKbmFile(utils::makeKbmString(2,
                                            1, 127,
                                            45,
                                            47, 330.0,
                                            2,
                                            {1, 2}));
        
        KeyboardMap::StoredKeyboardMap before(kb);
        
        SECTION("Bad mapping size.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(-1, //out of range
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         7,
                                                         {1})));
            REQUIRE(before.equals(kb));
        }
        SECTION("Bad middle note.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         -1, //out of range
                                                         5,6.0,
                                                         7,
                                                         {1})));
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         128, //out of range
                                                         5,6.0,
                                                         7,
                                                         {1})));
            REQUIRE(before.equals(kb));
        }
        SECTION("Bad frequency reference.")
        {
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,-1.0,//out of range
                                                         7,
                                                         {1})));
            REQUIRE(before.equals(kb));
                                           
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,0.0,//out of range
                                                         7,
                                                         {1})));
            REQUIRE(before.equals(kb));
        }
        SECTION("Bad formal octave scale degree")
        {
            KeyboardMap kb(10);
            before = KeyboardMap::StoredKeyboardMap(kb);
            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         11,//out of range
                                                         {1})));
            REQUIRE(before.equals(kb));

            REQUIRE_FALSE(kb.loadKbmString(utils::makeKbmString(1,
                                                         2,3,
                                                         4,
                                                         5,6.0,
                                                         -1,//out of range
                                                         {1})));
            REQUIRE(before.equals(kb));
        }
        
    }
    SECTION("Test mappings read correctly")
    {//TODO: something is fishy in this test. If we delete some of the notes in the .scl string, it still passes, even though it shouldn't.
        KeyboardMap kb(14);
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
                                                     0,
                                                     {7,6,5,4,3,2,1})));//7 mappings. should only be 6 here.
    }
    SECTION("Test mappings left out read as -1.")
    {
        REQUIRE(kb.loadKbmString(utils::makeKbmString(6,
                                               0,127,
                                               60,
                                               69,440.0,
                                               0,
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
    KeyboardMap kb(6);
    int numTests = 50;
    
    int numNotes = 6;
    int mapLowBound = 0; int mapHighBound = 127;
    int middleNote = 60;
    int refNote = 69; float refFreq= 440.;
    int formalOctaveScaleDegree=0;
    std::vector<int> mappings={1,2,3,4,5,6};

    for(int test = 0; test < numTests; test++)
    {
        middleNote = rand() % 128;
//        mapLowBound = rand() % 127;
//        mapHighBound = mapLowBound + rand() % (127 - mapLowBound);
        refNote = rand() % 128;
        formalOctaveScaleDegree = rand() % 6;
        
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
    KeyboardMap kb(6);
    int numTests = 50;
    
    int numNotes = 6;
    int mapLowBound = 0; int mapHighBound = 127;
    int middleNote = 60;
    int refNote = 69; float refFreq= 440.;
    int formalOctaveScaleDegree=0;
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


//TEST_CASE("Test KeyboardMap::modulate")
//{
//    int numRandomMods = 100000;
//
//    KeyboardMap kb;
//
//    kb.loadSclString("Pivot constant scale"
//                     "4"
//                     "1.1"
//                     "1.2"
//                     "1.3"
//                     "2."
//                     );
//
//    KeyboardMap afterMod;
//    int center;
//    int pivot;
//    for(int i = 0; i < numRandomMods; i++)
//    {
//        center = rand() % 128;
//        pivot = rand() % 128;
//
//        afterMod = kb;
//        afterMod.modulate(center, pivot);
//
//        SECTION()
//        {
//            REQUIRE(kb.getFreq(pivot) == afterMod.getFreq(pivot));
//        }
//    }
//}
