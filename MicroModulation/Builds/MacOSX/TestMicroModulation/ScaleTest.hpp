#pragma once

#define CATCH_CONFIG_MAIN

#include "Catch/catch_amalgamated.hpp"

#include "../MidiProcessor.h"

TEST_CASE( "Scale Files can be loaded") {
    
    MidiProcessor m;
    
    SECTION("test MidiProcessor::loadSclFile()"){
        SECTION("Bad path"){
            REQUIRE(!m.loadSclFile(""));
        }
        
        SECTION("Empty Scale"){
            REQUIRE(!m.loadSclFile(""));
        }
        
        SECTION("Num notes incorrect"){
            REQUIRE(!m.loadSclFile(""));
        }
        
        SECTION("Check num notes"){
            REQUIRE(!m.loadSclFile(""));
        }
        
        SECTION("Check ratio parsing"){
            REQUIRE(!m.loadSclFile(""));
        }
        
        SECTION("Check cents to ratio"){
            REQUIRE(!m.loadSclFile(""));
        }
    }
}
