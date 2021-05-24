/*
 ==============================================================================
 
 TestReadKbm.h
 Created: 23 May 2021 5:07:52pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include "../../MicroModulation/Source/KeyboardMap.h"

TEST_CASE("Test KeyboardMap::loadKbmFile")
{
    
    KeyboardMap kb;
    SECTION("Test meta values read correctly.")
    {
        kb.loadKbmString("1\n"
                         "2\n3\n"
                         "4\n"
                         "5\n6.0\n"
                         "7\n"
                         "1\n"//there are actually 7 mappings here, so should return false
                         );
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
        SECTION("Bad mapping size.")
        {
            REQUIRE_FALSE(kb.loadKbmString("-1\n"
                                           "2\n3\n"
                                           "4\n"
                                           "5\n6.0\n"
                                           "7\n"
                                           "0\n"
                                           ));
        }
        SECTION("Bad middle note.")
        {
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "-1\n"
                                           "5\n6.0\n"
                                           "7\n"
                                           "0\n"
                                           ));
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "128\n"
                                           "5\n6.0\n"
                                           "7\n"
                                           "0\n"
                                           ));
        }
        SECTION("Bad frequency reference.")
        {
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "4\n"
                                           "5\n-1.0\n"
                                           "7\n"
                                           "0\n"
                                           ));
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "0\n"
                                           "5\n0.0\n"
                                           "7\n"
                                           "0\n"
                                           ));
            
            
        }
        SECTION("Bad formal octave scale degree")
        {
            
            kb.getNotes().clear();
            for(int i = 0; i < 10; i++){
                kb.getNotes().push_back(i);
            }
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "4\n"
                                           "5\n6.0\n"
                                           "11\n"
                                           "0\n"
                                           ));
            REQUIRE_FALSE(kb.loadKbmString("1\n"
                                           "2\n3\n"
                                           "4\n"
                                           "5\n6.0\n"
                                           "-1\n"
                                           "0\n"
                                           ));
            
            
        }
        
    }
    SECTION("Test mappings read correctly")
    {//TODO: something is fishy in this test. If we delete some of the notes in the .scl string, it still passes, even though it shouldn't.
        kb.loadSclString("15\n"//we need to load enough notes so that notes.size() > 14 (this is because our mappings go up to the 14th scale degree.
                         "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n"
                         );
        kb.loadKbmString("5\n"
                         "2\n3\n"
                         "4\n"
                         "5\n6.0\n"
                         "7\n"
                         "10\n11\n12\n13\n14\n"
                         );
        for(int i = 0; i < 5; i++)
        {
            REQUIRE(kb.getMapping(i) == i + 10); //the i+10 is just because  I set the file up so this loop would work.
        }
    }
    SECTION("Test too many mappings.")
    {
        REQUIRE_FALSE(kb.loadKbmString("6\n"
                                       "0\n127\n"
                                       "60\n"
                                       "69\n440.0\n"
                                       "0\n"
                                       "7\n6\n5\n4\n3\n2\n1\n"//there are actually 7 mappings here, so should return false
                                       ));
    }
    SECTION("Test mappings left out read as -1.")
    {
        REQUIRE(kb.loadKbmString("6\n"
                                 "0\n127\n"
                                 "60\n"
                                 "69\n440.0\n"
                                 "0\n"
                                 ));
        int mappingSize = 6;
        REQUIRE(kb.getMapping().size() == 6);
        for(int i = 0; i < mappingSize; i++)
        {
            REQUIRE(kb.getMapping(i) == -1);
        }
    }
    
}

//TEST_CASE("Test KeyboardMap::getFreq")
//{
//
//}


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
