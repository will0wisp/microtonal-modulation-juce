#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Catch/catch_amalgamated.hpp"
//User-written Code
#include "../../MicroModulation/Source/Scale.h"
#include "../../MicroModulation/Source/utils.h"

TEST_CASE("Scale::getNotes() works correctly.")
{
    juce::UndoManager um;
    Scale s(um);
    SECTION("Upon creation, getNotes() is empty")
    {
        REQUIRE(s.getNotes().size() == 0);
    }
    
}

TEST_CASE( "Scale (.scl) Files can be loaded") {
    
    juce::UndoManager um;
    Scale scale(um);

    juce::ValueTree before = scale.scaleValues.createCopy();

    SECTION("test Scale::loadSclFile()"){
        SECTION("Given bad path"){
            REQUIRE_FALSE(scale.loadSclFile(""));
            REQUIRE(scale.scaleValues.isEquivalentTo(before));
        }

        // A file with 0 notes should result in 1 note in the scale (with ratio 1.0).
        SECTION("Test File with 0 Notes"){
            REQUIRE(
                    scale.loadSclString(
                                           "0 Note Scale\n"
                                           "0"));
            REQUIRE(scale.getNotes().size() == 1);
            REQUIRE((float)scale.getNotes().getUnchecked(0) == 1.0);
        }

        SECTION("Test read scale description"){
            REQUIRE(scale.loadSclString(
                                           "Description\n"
                                           "0"));
            REQUIRE(scale.getDescription() == "Description");
        }
        SECTION("Test read empty scale description"){
            REQUIRE(scale.loadSclString(
                                           "\n"
                                           "0"));
            REQUIRE(scale.getDescription() == "");
        }

        SECTION("Num notes in file incorrect"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Wrong Number Note Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
            REQUIRE(scale.scaleValues.isEquivalentTo(before));
        }

        SECTION("Check num notes correct"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Check num notes correct Scale\n"
                                                 "1\n"
                                                 "1.2\n"
                                                 "1.3\n"));
            REQUIRE(scale.scaleValues.isEquivalentTo(before));
        }
        SECTION("Check negative note ratio"){
            REQUIRE_FALSE(scale.loadSclString(
                                                 "Check negative note ratio Scale\n"
                                                 "3\n"
                                                 "-1/2\n"
                                                 "1.3\n"
                                                 "3"));
            REQUIRE(scale.scaleValues.isEquivalentTo(before));
        }

        SECTION("Check ratio parsing"){
            for(int i = 0; i < 10; i++){
            REQUIRE(scale.loadSclString(
                                           "Check ratio parsing Scale\n"
                                           "4\n"
                                           "1/2\n"
                                           "2/3\n"
                                           "3/7\n"
                                           "19/11\n"));
            REQUIRE(scale.getNoteRatio(0) == Catch::Approx(1.0/2.0));
            REQUIRE(scale.getNoteRatio(1) == Catch::Approx(2.0/3.0));
            REQUIRE(scale.getNoteRatio(2)  == Catch::Approx(3.0/7.0));
            REQUIRE(scale.getNoteRatio(3)  == Catch::Approx(19.0/11.0));
            }
        }

        SECTION("Check integer interpreted as ratio"){ // integers without a decimal should be ratios
            REQUIRE(scale.loadSclString(
                                           "Check integer is ratio  Scale\n"
                                           "2\n"
                                           "2\n"
                                           "2.\n"));
            REQUIRE(scale.getNotes().getUnchecked(0) == Catch::Approx(2));
            REQUIRE(scale.getNotes().getUnchecked(1) == Catch::Approx(1.001156)); //calculated using cents -> ratio formula. 1.00116 is a ratio of 2 cents, about.
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
            REQUIRE(scale.getNotes().getUnchecked(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().getUnchecked(3) == Catch::Approx(3.0f/2.0f));
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
            REQUIRE(scale.getNotes().getUnchecked(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().getUnchecked(3) == Catch::Approx(3.0f/2.0f));
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
            REQUIRE(scale.getNotes().getUnchecked(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().getUnchecked(3) == Catch::Approx(3.0f/2.0f));
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
            REQUIRE(scale.getNotes().getUnchecked(0) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(1) == Catch::Approx(2.));
            REQUIRE(scale.getNotes().getUnchecked(2) == Catch::Approx(4.));
            REQUIRE(scale.getNotes().getUnchecked(3) == Catch::Approx(3.0f/2.0f));
        }
    }

    SECTION("test Scale::hasSclLoaded")
    {
        REQUIRE(!scale.hasSclLoaded());
        scale.loadSclString("0 Note Scale\n"
                            "0");
        REQUIRE(scale.hasSclLoaded());
        
    }
}


//TODO: improove coverage. this is absolutely atrocious.
TEST_CASE("Midi notes can be converted into frequencies.")
{
    juce::UndoManager um;
    Scale s(um);
    
    SECTION("12-TET 12 note representation")
    {
        s.loadSclString(utils::makeSclString("12-TET 12 notes", "12", {"100.0", "200.0", "300.0", "400.0", "500.0", "600.0", "700.0", "800.0", "900.0", "1000.0", "1100.0", "1200.0"}));
        
        for(int noteNum = 0; noteNum < 128; noteNum++)
        {
            REQUIRE(s.getFreq(noteNum) == Catch::Approx(utils::getMidiNoteInHertz(noteNum, 440.0)) );
        }
    }
    SECTION("12-TET 1 note representation")
    {
        s.loadSclString(utils::makeSclString("12-TET 1 note", "1", {"100.0"}));
        for(int noteNum = 0; noteNum < 128; noteNum++)
        {
            REQUIRE(s.getFreq(noteNum) == Catch::Approx(utils::getMidiNoteInHertz(noteNum, 440.0)) );
        }
    }
    SECTION("Random 3-note scales") {
        int numTests = 50;
        std::srand(time(NULL));
        
        juce::UndoManager um;
        
        int numRightOctaves = 0;
        for(int i = 0; i < numTests; i++)
        {
            Scale s(um);
            int middleNote = utils::randInt(0, 128);
            int refNote = middleNote;
            float refFreq = utils::randFloat(.1, 5000.0);
            
            std::vector<float> scaleRatios = {utils::randFloat(1.0, 3.0),utils::randFloat(1.0, 3.0),utils::randFloat(1.0, 3.0)} ;
            
            
            bool sclLoaded = s.loadSclString(utils::makeSclString("3 note scale test",
                                                                  "3",
                                                                  scaleRatios));
            bool kbmLoaded = s.loadKbmString(utils::makeKbmString(3,
                                                                  0, 127, //bounds
                                                                  middleNote, //midle note
                                                                  refNote, refFreq, //refnote, freq
                                                                  3, //formal octave
                                                                  {0, 1, 2}));
            REQUIRE(sclLoaded);
            REQUIRE(kbmLoaded);
            
            float octaveRatio = s.getNoteRatio(s.getKeyboardMap().getFormalOctaveScaleDegree());
            if(octaveRatio == Catch::Approx(2))
            {
                numRightOctaves++;
            }else {
                break;
            }
               
            int curNoteCounter = 1;
            float freq = refFreq;
            for(int i = middleNote+1; i < 128; i++ )
            {
                switch(curNoteCounter++)
                {
                    case(0):
                        freq = freq = freq / scaleRatios[1] * scaleRatios[2];
                        break;
                    case(1):
                        freq = freq * scaleRatios[0];
                        break;
                    case(2):
                        freq = freq / scaleRatios[0] * scaleRatios[1];
                        curNoteCounter = 0;
                        break;
                }
                float scaleFreq = s.getFreq(i);
                REQUIRE(scaleFreq == Catch::Approx(freq));
            }
            freq = refFreq;
            curNoteCounter = 2;
            for(int i = middleNote - 1; i >=0; i--)
            {
                switch(curNoteCounter--)
                {
                    case(0):
                        freq = freq/ scaleRatios[0];
                        curNoteCounter = 2;
                        break;
                    case(1):
                        freq = freq / scaleRatios[1] * scaleRatios[0];
                        break;
                    case(2):
                        freq = freq / scaleRatios[2] * scaleRatios[1];
                        break;
                }
                float scaleFreq = s.getFreq(i);
                REQUIRE(scaleFreq == Catch::Approx(freq));
            }
        }
    }
        
}
