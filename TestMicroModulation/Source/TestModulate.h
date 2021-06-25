/*
 ==============================================================================
 
 TestModulate.h
 Created: 21 May 2021 2:30:46pm
 Author:  Willow Weiner
 
 ==============================================================================
 */

#pragma once

#include "Catch/catch_amalgamated.hpp"
//User-written Code
#include "../../MicroModulation/Source/MidiProcessor.h"


TEST_CASE("The user can modulate between scales")
{
    juce::UndoManager um;
    MidiProcessor m(um);
    
    SECTION("Modulate on Empty Scale")
    {
    }
    
    SECTION("Test modulation frequencies")
    {
        
    }
    SECTION("Test pivot of modulation stays same frequency")
    {
        //int numTests = 1000;
        m.scale.loadSclString(
                              "Check center stays same scale\n"
                              "5\n"
                              "6/5\n"
                              "5/4\n"
                              "4/3\n"
                              "3/2\n"
                              "2");
        juce::int8 middleNote = m.scale.getKeyboardMap().getMiddleNote();
        int scaleSize = m.scale.getKeyboardMap().getMapping().size();
        
        SECTION("Pivot and Center in same octave")
        {
            SECTION("Both above middleNote")
            {
                int highNote = middleNote + scaleSize*2 - 1;
                int lowNote = middleNote + scaleSize + 1;
                SECTION("pivot > center")
                {
                    float initPivotFreq = m.scale.getFreq(highNote);
                    m.scale.modulate(lowNote, highNote);
                    float afterModPivotFreq = m.scale.getFreq(highNote);
                    REQUIRE(initPivotFreq == afterModPivotFreq);
                }
                SECTION("center > pivot")
                {
                    float initPivotFreq = m.scale.getFreq(lowNote);
                    m.scale.modulate(highNote, lowNote);
                    float afterModPivotFreq = m.scale.getFreq(lowNote);
                    REQUIRE(initPivotFreq == afterModPivotFreq);
                }
            }
            
            SECTION("Both below middleNote")
            {
            }
            SECTION("Both in middleNote's octave")
            {
                
            }
        }
        SECTION("Pivot and Center in different octaves")
        {
            SECTION("pivot > center")
            {}
            SECTION("center > pivot")
            {}
        }
    }
}



TEST_CASE("There are undo, redo, and saved state functionalities")
{
    
    SECTION("Test undo function")
    {
        
    }
    
    SECTION("Test redo function")
    {
        
    }
    
    SECTION("Test recall function")
    {
        
    }
}
