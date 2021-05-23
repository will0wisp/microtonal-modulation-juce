#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Catch/catch_amalgamated.hpp"
//User-written Code
#include "../../MicroModulation/Source/MidiProcessor.h"
#include "TestUtilities.h"


TEST_CASE( "Scale (.scl) Files can be loaded") {
    
    MidiProcessor m;

    SECTION("test MidiProcessor::loadSclFile()"){
        SECTION("Given bad path"){
            REQUIRE_FALSE(m.loadSclFile(""));
        }
        
        // A file with 0 notes should result in 1 note in the scale (with ratio 1.0).
        SECTION("Test File with 0 Notes"){
            REQUIRE(
                    testUtility::loadScale(m,
                                           "0 Note Scale\n"
                                           "0"));
            REQUIRE(m.scale.getNotes().size() == 1);
            REQUIRE(m.scale.getNotes().at(0) == 1.0);
        }
        
        SECTION("Test read scale description"){
            REQUIRE(testUtility::loadScale(m,
                                           "Description\n"
                                           "0"));
            REQUIRE(m.scale.getDescription() == "Description");
        }
        
        SECTION("Num notes in file incorrect"){
            REQUIRE_FALSE(testUtility::loadScale(m,
                                                 "Wrong Number Note Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
        }
        
        SECTION("Check num notes correct"){
            REQUIRE_FALSE(testUtility::loadScale(m,
                                                 "Check num notes correct Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
        }
        SECTION("Check negative note ratio"){
            REQUIRE_FALSE(testUtility::loadScale(m,
                                                 "Check negative note ratio Scale\n"
                                                 "3\n"
                                                 "-1/2\n"
                                                 "1.3\n"
                                                 "3"));
        }
        
        SECTION("Check ratio parsing"){
            REQUIRE(testUtility::loadScale(m,
                                           "Check ratio parsing Scale\n"
                                           "4\n"
                                           "1/2\n"
                                           "2/3\n"
                                           "3/7\n"
                                           "19/11\n"));
            REQUIRE(m.scale.getNotes().at(0) == Catch::Approx(1.0/2.0));
            REQUIRE(m.scale.getNotes().at(1) == Catch::Approx(2.0/3.0));
            REQUIRE(m.scale.getNotes().at(2) == Catch::Approx(3.0/7.0));
            REQUIRE(m.scale.getNotes().at(3) == Catch::Approx(19.0/11.0));
        }
        
        SECTION("Check integer interpreted as ratio"){ // integers without a decimal should be ratios
            REQUIRE(testUtility::loadScale(m,
                                           "Check integer is ratio  Scale\n"
                                           "2\n"
                                           "2\n"
                                           "2.\n"));
            REQUIRE(m.scale.getNotes().at(0) == Catch::Approx(2));
            REQUIRE(m.scale.getNotes().at(1) == Catch::Approx(1.001156)); //calculated using cents -> ratio formula. 1.00116 is a ratio of 2 cents, about.
        }
        
        SECTION("Check cents parsing"){
            REQUIRE(testUtility::loadScale(m,
                                           "Check cents parsing Scale\n"
                                           "4\n"
                                           "1200.\n" //octave. should be ratio of 2
                                           "1200.0\n" //octave. should be ratio of 2
                                           "2400.\n" //2 octaves. should be ratio of 4
                                           "701.955\n" //P5. should be ratio of 3/2
                                           ));
            REQUIRE(m.scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(m.scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        
        //TODO make this dependency part of the testing.
        //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
        SECTION("Test ignore comment lines"){ // comments start with '!'
            REQUIRE(testUtility::loadScale(m,
                                           "!comment\n"
                                           "!comment\n"
                                           "    !comment\n"
                                           "Test ignore comment lines Scale\n"
                                           "!comment\n"
                                           "4\n"
                                           "!comment\n"
                                           "1200.\n" //octave. should be ratio of 2
                                           "!comment\n"
                                           "!comment\n"
                                           "1200.0\n" //octave. should be ratio of 2
                                           "2400.0\n" //2 octaves. should be ratio of 4
                                           "!comment\n"
                                           "701.955\n" //P5. should be ratio of 3/2
                                           "!comment\n"
                                           "!comment\n"
                                           "!comment\n"
                                           "!comment\n"));
            REQUIRE(m.scale.getDescription() == "Test ignore comment lines Scale");
            REQUIRE(m.scale.getNotes().size() == 4);
            REQUIRE(m.scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(m.scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        //TODO make this dependency part of the testing
        //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
        SECTION("Test ignore inline comments"){ //
            REQUIRE(testUtility::loadScale(m,
                                           "Test ignore inline comments scale!comment\n"
                                           "4!comment\n"
                                           "1200.!comment\n"                //octave. should be ratio of 2
                                           "1200.0              !comment\n" //octave. should be ratio of 2
                                           "4!/2\n"                         //should be a ratio of 4
                                           "701.955 !comment\n"             //P5. should be ratio of 3/2
                                           ));
            REQUIRE(m.scale.getDescription() == "Test ignore inline comments scale");
            REQUIRE(m.scale.getNotes().size() == 4);
            REQUIRE(m.scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(m.scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(m.scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
    }
}


TEST_CASE("Keyboard mapping (.kbm) Files can be loaded")
{
    
}
