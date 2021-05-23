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
    MidiProcessor m;
    
    SECTION("Modulate on Empty Scale")
    {
    }
    
    SECTION("Test modulation frequencies")
    {
        
    }
    SECTION("Test center stays same frequency")
    {
        m.scale.loadSclString(
                              "Check center stays same scale\n"
                              "4\n"
                              "6/5\n"
                              "5/4\n"
                              "4/3\n"
                              "3/2\n");
        
        
    }
}

TEST_CASE("Midi can be converted into frequency")
{
    
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
