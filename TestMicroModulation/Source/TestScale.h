#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Catch/catch_amalgamated.hpp"
//User-written Code
#include "../../MicroModulation/Source/Scale.h"
#include "../../MicroModulation/Source/utils.h"

TEST_CASE( "Scale (.scl) Files can be loaded") {
    
    Scale scale;
    Scale::StoredScale before = Scale::StoredScale(scale);

    
    //TODO: define equals ('==') operator
    
    //TODO: include a StoredScale store(Scale s) function.
    SECTION("test Scale::loadSclFile()"){
        SECTION("Given bad path"){
            REQUIRE_FALSE(scale.loadSclFile(""));
            REQUIRE(before.equals(scale));
        }
        
        // A file with 0 notes should result in 1 note in the scale (with ratio 1.0).
        SECTION("Test File with 0 Notes"){
            REQUIRE(
                    scale.loadSclString(
                                           "0 Note Scale\n"
                                           "0"));
            REQUIRE(scale.getNotes().size() == 1);
            REQUIRE(scale.getNotes().at(0) == 1.0);
        }
        
        SECTION("Test read scale description"){
            REQUIRE(scale.loadSclString(
                                           "Description\n"
                                           "0"));
            REQUIRE(scale.getDescription() == "Description");
        }
        
        SECTION("Num notes in file incorrect"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Wrong Number Note Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
            REQUIRE(before.equals(scale));
        }
        
        SECTION("Check num notes correct"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Check num notes correct Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
            REQUIRE(before.equals(scale));
        }
        SECTION("Check negative note ratio"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Check negative note ratio Scale\n"
                                                 "3\n"
                                                 "-1/2\n"
                                                 "1.3\n"
                                                 "3"));
            REQUIRE(before.equals(scale));
        }
        
        SECTION("Check ratio parsing"){
            REQUIRE(scale.loadSclString(
                                           "Check ratio parsing Scale\n"
                                           "4\n"
                                           "1/2\n"
                                           "2/3\n"
                                           "3/7\n"
                                           "19/11\n"));
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(1.0/2.0));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(2.0/3.0));
            REQUIRE(scale.getNotes().at(2) == Catch::Approx(3.0/7.0));
            REQUIRE(scale.getNotes().at(3) == Catch::Approx(19.0/11.0));
        }
        
        SECTION("Check integer interpreted as ratio"){ // integers without a decimal should be ratios
            REQUIRE(scale.loadSclString(
                                           "Check integer is ratio  Scale\n"
                                           "2\n"
                                           "2\n"
                                           "2.\n"));
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(2));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(1.001156)); //calculated using cents -> ratio formula. 1.00116 is a ratio of 2 cents, about.
        }
        
        SECTION("Check cents parsing"){
            REQUIRE(scale.loadSclString(
                                           "Check cents parsing Scale\n"
                                           "4\n"
                                           "1200.\n" //octave. should be ratio of 2
                                           "1200.0\n" //octave. should be ratio of 2
                                           "2400.\n" //2 octaves. should be ratio of 4
                                           "701.955\n" //P5. should be ratio of 3/2
                                           ));
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        
        //TODO make this dependency part of the testing.
        //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
        SECTION("Test ignore comment lines"){ // comments start with '!'
            REQUIRE(scale.loadSclString(
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
            REQUIRE(scale.getDescription() == "Test ignore comment lines Scale");
            REQUIRE(scale.getNotes().size() == 4);
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        //TODO make this dependency part of the testing
        //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
        SECTION("Test ignore inline comments"){ //
            REQUIRE(scale.loadSclString(
                                           "Test ignore inline comments scale!comment\n"
                                           "4!comment\n"
                                           "1200.!comment\n"                //octave. should be ratio of 2
                                           "1200.0              !comment\n" //octave. should be ratio of 2
                                           "4!/2\n"                         //should be a ratio of 4
                                           "701.955 !comment\n"             //P5. should be ratio of 3/2
                                           ));
            REQUIRE(scale.getDescription() == "Test ignore inline comments scale");
            REQUIRE(scale.getNotes().size() == 4);
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        SECTION("Test ignore line after notes"){ //
            REQUIRE(scale.loadSclString(
                                           "Test ignore inline comments scale!comment\n"
                                           "4\n"
                                           "1200.\n"                //octave. should be ratio of 2
                                           "1200.0xcj lkdjfx zlckdsf\n" //octave. should be ratio of 2
                                           "4/1.999\n"               //should be a ratio of 4, as the ".9999" should be ignored
                                           "701.955 xldkfj\n"             //P5. should be ratio of 3/2
                                           ));
            REQUIRE(scale.getDescription() == "Test ignore inline comments scale");
            REQUIRE(scale.getNotes().size() == 4);
            REQUIRE(scale.getNotes().at(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().at(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().at(3) == Catch::Approx(3.0f/2.0f));
        }
    }
}

TEST_CASE("Midi notes can be converted into frequencies.")
{
    Scale s;
    s.loadSclString(utils::makeSclString("12-TET", "12", {"100.0", "200.0", "300.0", "400.0", "500.0", "600.0", "700.0", "800.0", "900.0", "1000.0", "1100.0", "1200.0"}));
    
    for(int noteNum = 60; noteNum < 128; noteNum++)
    {
        REQUIRE(s.getFreq(noteNum) == Catch::Approx((utils::getMidiNoteInHertz(noteNum, 440.0))) );
    }
                                        
}
