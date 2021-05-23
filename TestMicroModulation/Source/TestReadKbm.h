/*
 ==============================================================================
 
 TestReadKbm.h
 Created: 23 May 2021 5:07:52pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include "../../MicroModulation/Source/KeyboardMap.h"

TEST_CASE("Keyboard mapping (.kbm) Files can be loaded")
{
    
    KeyboardMap kbMap;
    SECTION("Test meta values read correctly.")
    {
        kbMap.loadKbmString("1\n"
                            "2\n3\n"
                            "4\n"
                            "5\n6.0\n"
                            "7\n"
                            "1\n"//there are actually 7 mappings here, so should return false
                            );
        SECTION("Test range to remap")
        {
            REQUIRE(kbMap.getRangeToRetune().first == 2);
            REQUIRE(kbMap.getRangeToRetune().second == 3);
        }
        SECTION("Test middle note read correctly")
        {
            REQUIRE(kbMap.getMiddleNote() == 4);
        }
        SECTION("Test midi reference note and freq read correctly")
        {
            REQUIRE(kbMap.getRefererenceMidiFreqPair().first == 5);
            REQUIRE(kbMap.getRefererenceMidiFreqPair().second == 6.0);
        }
        SECTION("Test formal octave read correctly")
        {
            REQUIRE(kbMap.getFormalOctaveScaleDegree() == 7);
        }
    }
    SECTION("Test mappings read correctly")
    {
        kbMap.loadKbmString("5\n"
                            "2\n3\n"
                            "4\n"
                            "5\n6.0\n"
                            "7\n"
                            "10\n11\n12\n13\n14\n"
                            );
        for(int i = 0; i < 5; i++)
        {
            REQUIRE(kbMap.getMapping(i) == i + 10); //the i+10 is just because  I set the file up so this loop would work.
        }
    }
    SECTION("Test too many mappings.")
    {
        REQUIRE_FALSE(kbMap.loadKbmString("6\n"
                                          "0\n127\n"
                                          "60\n"
                                          "69\n440.0\n"
                                          "0\n"
                                          "7\n6\n5\n4\n3\n2\n1\n"//there are actually 7 mappings here, so should return false
                                          ));
    }
    SECTION("Test mappings left out read as -1.")
    {
        REQUIRE(kbMap.loadKbmString("6\n"
                                    "0\n127\n"
                                    "60\n"
                                    "69\n440.0\n"
                                    "0\n"
                                    ));
        int mappingSize = 6;
        REQUIRE(kbMap.getMapping().size() == 6);
        for(int i = 0; i < mappingSize; i++)
        {
            REQUIRE(kbMap.getMapping(i) == -1);
        }
    }

}
