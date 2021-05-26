///*
// ==============================================================================
// 
// TestReadKbm.h
// Created: 23 May 2021 5:07:52pm
// Author:  Willow Weiner
// 
// ==============================================================================
// */
//
//#pragma once
//
//#define private public // so we can test private member functions
//#include "../../MicroModulation/Source/KeyboardMap.h"
//#include <stdio.h>
//
//std::string makeKbmString(int numNotes,
//                     int mapLowBound, int mapHighBound,
//                     int middleNote,
//                     int refNote, float refFreq,
//                     int formalOctaveScaleDegree,
//                     std::vector<int> mappings)
//{
//    std::string output = std::to_string(numNotes) + "\n"
//    + std::to_string(mapLowBound) + "\n" + std::to_string(mapHighBound) + "\n"
//    + std::to_string(middleNote) + "\n"
//    + std::to_string(refNote) + "\n" + std::to_string(refFreq) + "\n"
//    + std::to_string(formalOctaveScaleDegree) + "\n";
//    for(int i : mappings)
//    {
//        output = output + std::to_string(i) + "\n";
//    }
//    return output;
//}
//
//
//TEST_CASE("Test KeyboardMap::loadKbmFile")
//{
//    
//    KeyboardMap kb;
//    SECTION("Test meta values read correctly.")
//    {
//        kb.loadKbmString(makeKbmString(1,
//                                       2,3,
//                                       4,
//                                       5,6.0,
//                                       7,
//                                       {1}));
//        SECTION("Test range to remap")
//        {
//            REQUIRE(kb.getRangeToRetune().first == 2);
//            REQUIRE(kb.getRangeToRetune().second == 3);
//        }
//        SECTION("Test middle note read correctly")
//        {
//            REQUIRE(kb.getMiddleNoteFreqPair().first == 4);
//        }
//        SECTION("Test midi reference note and freq read correctly")
//        {
//            REQUIRE(kb.getRefererenceMidiFreqPair().first == 5);
//            REQUIRE(kb.getRefererenceMidiFreqPair().second == 6.0);
//        }
//        SECTION("Test formal octave read correctly")
//        {
//            REQUIRE(kb.getFormalOctaveScaleDegree() == 7);
//        }
//    }
//    SECTION("Test bad meta values.")
//    {
//        SECTION("Bad mapping size.")
//        {
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(-1, //out of range
//                                                         2,3,
//                                                         4,
//                                                         5,6.0,
//                                                         7,
//                                                         {1})));
//        }
//        SECTION("Bad middle note.")
//        {
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         -1, //out of range
//                                                         5,6.0,
//                                                         7,
//                                                         {1})));
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         128, //out of range
//                                                         5,6.0,
//                                                         7,
//                                                         {1})));
//        }
//        SECTION("Bad frequency reference.")
//        {
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         4,
//                                                         5,-1.0,//out of range
//                                                         7,
//                                                         {1})));
//                                           
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         4,
//                                                         5,0.0,//out of range
//                                                         7,
//                                                         {1})));
//            
//            
//        }
//        SECTION("Bad formal octave scale degree")
//        {
//            
//            kb.getNotes().clear();
//            for(int i = 0; i < 10; i++){
//                kb.getNotes().push_back(i);
//            }
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         4,
//                                                         5,6.0,
//                                                         11,//out of range
//                                                         {1})));
//            REQUIRE_FALSE(kb.loadKbmString(makeKbmString(1,
//                                                         2,3,
//                                                         4,
//                                                         5,6.0,
//                                                         -1,//out of range
//                                                         {1})));
//        }
//        
//    }
//    SECTION("Test mappings read correctly")
//    {//TODO: something is fishy in this test. If we delete some of the notes in the .scl string, it still passes, even though it shouldn't.
//        kb.loadSclString("15\n"//we need to load enough notes so that notes.size() > 14 (this is because our mappings go up to the 14th scale degree.
//                         "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n"
//                         );
//        kb.loadKbmString(makeKbmString(5,
//                                       2,3,
//                                       4,
//                                       5,6.0,
//                                       7,//out of range
//                                       {10,11,12,13,14}));
//        for(int i = 0; i < 5; i++)
//        {
//            REQUIRE(kb.getMapping(i) == i + 10); //the i+10 is just because  I set the file up so this loop would work.
//            REQUIRE(kb.mapping.size() == 5);
//        }
//    }
//    SECTION("Test too many mappings.")
//    {
//        REQUIRE_FALSE(kb.loadKbmString(makeKbmString(6,
//                                                     0,127,
//                                                     60,
//                                                     69,440.0,
//                                                     0,
//                                                     {7,6,5,4,3,2,1})));//7 mappings. should only be 6 here.
//    }
//    SECTION("Test mappings left out read as -1.")
//    {
//        REQUIRE(kb.loadKbmString(makeKbmString(6,
//                                               0,127,
//                                               60,
//                                               69,440.0,
//                                               0,
//                                               {})));
//        int mappingSize = 6;
//        REQUIRE(kb.getMapping().size() == 6);
//        for(int i = 0; i < mappingSize; i++)
//        {
//            REQUIRE(kb.getMapping(i) == -1);
//        }
//    }
//    
//}
//
//
//TEST_CASE("Test KeyboardMap::getScaleDegree")
//{
//    KeyboardMap kb;
//    int numTests = 50;
//    
//    int numNotes = 6;
//    int mapLowBound = 0; int mapHighBound = 127;
//    int middleNote = 60;
//    int refNote = 69; float refFreq= 440.;
//    int formalOctaveScaleDegree=0;
//    std::vector<int> mappings={1,2,3,4,5,6};
//    kb.loadSclString("getScaleDegree test scale\n"
//                         "1\n2\n3\n4\n5\n6\n"
//                         );
//        
//    for(int test = 0; test < numTests; test++)
//    {
//        middleNote = rand() % 128;
////        mapLowBound = rand() % 127;
////        mapHighBound = mapLowBound + rand() % (127 - mapLowBound);
//        refNote = rand() % 128;
//        formalOctaveScaleDegree = rand() % 6;
//        
//        assert(kb.loadKbmString(makeKbmString(numNotes, mapLowBound, mapHighBound, middleNote, refNote, refFreq, formalOctaveScaleDegree, mappings)));
//        REQUIRE(kb.getScaleDegree(middleNote) == 1); //tests middleNote is the first mapped note
//        
//        //this for loop tests the mapping of every other note
//        for(int offset = 0; offset < numNotes; offset++ ){//'offset+1' should map to mappings.at(i)
//            for(int i = middleNote + offset; i < 128; i += numNotes)
//            {
//                REQUIRE(kb.getScaleDegree(i) == offset+1);//midiNote >= middleNote
//            }
//            for(int i = middleNote + offset; i >= 0 && i <128; i -= numNotes)
//            {
//                REQUIRE(kb.getScaleDegree(i) == offset+1);//midiNote < middleNote
//            }
//        }
//    }
//}
//TEST_CASE("Test KeyboardMap::getOctave")
//{
//    KeyboardMap kb;
//    int numTests = 50;
//    
//    int numNotes = 6;
//    int mapLowBound = 0; int mapHighBound = 127;
//    int middleNote = 60;
//    int refNote = 69; float refFreq= 440.;
//    int formalOctaveScaleDegree=0;
//    std::vector<int> mappings={1,2,3,4,5,6};
//    
//    kb.loadSclString("getScaleDegree test scale\n"
//                         "1\n2\n3\n4\n5\n6\n"
//                         );
//    
//    for(int test = 1; test <= numTests; test++){
//        assert(kb.loadKbmString(makeKbmString(numNotes, mapLowBound, mapHighBound, middleNote, refNote, refFreq, formalOctaveScaleDegree, mappings)));
//        
//        int octave = 0;
//        for(int i = middleNote; i < 128; i+= numNotes)
//        {
//            for(int j = 0; j < numNotes && i+j < 128; j++)
//            {
//                REQUIRE(kb.getOctave(i+j) == octave);
//            }
//            octave++;
//        }
//        octave = 0;
//        for(int i = middleNote; i>=0 && i < 128; i-= numNotes)
//        {
//            for(int j = 0; j < numNotes && i+j < 128; j++)
//            {
//                REQUIRE(kb.getOctave(i+j) == octave);
//            }
//            octave--;
//        }
//    }
//
//}
//
//
////TEST_CASE("Test KeyboardMap::modulate")
////{
////    int numRandomMods = 100000;
////
////    KeyboardMap kb;
////
////    kb.loadSclString("Pivot constant scale"
////                     "4"
////                     "1.1"
////                     "1.2"
////                     "1.3"
////                     "2."
////                     );
////
////    KeyboardMap afterMod;
////    int center;
////    int pivot;
////    for(int i = 0; i < numRandomMods; i++)
////    {
////        center = rand() % 128;
////        pivot = rand() % 128;
////
////        afterMod = kb;
////        afterMod.modulate(center, pivot);
////
////        SECTION()
////        {
////            REQUIRE(kb.getFreq(pivot) == afterMod.getFreq(pivot));
////        }
////    }
////}
