#define CATCH_CONFIG_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "/Users/NevetssteveN/Desktop/Development/Catch/catch_amalgamated.hpp"

#include "/Users/NevetssteveN/Desktop/Development/MicroModulation/MicroModulation/Source/MidiProcessor.h"

TEST_CASE( "Scale (.scl) Files can be loaded") {
    
    MidiProcessor m;
    
    //making a temporary file to write scales to in tests
    char* tmpfilePath;
    mkstemp(tmpfilePath);
    unlink(tmpfilePath);
    FILE* tmpFile = fopen(tmpfilePath, "w");
    
    SECTION("test MidiProcessor::loadSclFile()"){
        SECTION("Given bad path"){
            REQUIRE_FALSE(m.loadSclFile(""));
        }
        
        // A file with 0 notes should result in 1 note in the scale (with ratio 1.0).
        SECTION("Test File with 0 Notes"){
            fputs ("0 Note Scale\n"
                   "0"
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE(m.loadSclFile(tmpfilePath));
            REQUIRE(m.scale.notes.size() == 1);
            REQUIRE(m.scale.notes.at(0) == 1.0);
        }
        SECTION("Test read scale description"){
            fputs ("Description\n"
                   "0", tmpFile);
            fclose(tmpFile);
            REQUIRE(m.loadSclFile(tmpfilePath));
            REQUIRE(m.scale.description == "Description");
        }
        
        SECTION("Num notes in file incorrect"){
            fputs ("Wrong Number Note Scale\n"
                   "1\n"
                   "1.2\n"
                   "1.3\n"
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE_FALSE(m.loadSclFile(tmpfilePath));
        }
        
        SECTION("Check num notes correct"){
            fputs ("Check num notes correct Scale\n"
                   "1\n"
                   "1.2\n"
                   "1.3\n"
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE_FALSE(m.loadSclFile(tmpfilePath));
        }
        
        SECTION("Check ratio parsing"){
            fputs ("Check ratio parsing Scale\n"
                   "4\n"
                   "1/2\n"
                   "2/3\n"
                   "3/7\n"
                   "19/11\n"
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE(m.loadSclFile(tmpfilePath));
            REQUIRE(m.scale.notes.at(0) == Catch::Approx(1.0/2.0));
            REQUIRE(m.scale.notes.at(1) == Catch::Approx(2.0/3.0));
            REQUIRE(m.scale.notes.at(2) == Catch::Approx(3.0/7.0));
            REQUIRE(m.scale.notes.at(3) == Catch::Approx(19.0/11.0));
        }
        
        SECTION("Check cents parsing"){
            fputs ("Check cents parsing Scale\n"
                   "4\n"
                   "1200\n" //octave. should be ratio of 2
                   "1200.0\n" //octave. should be ratio of 2
                   "2400\n" //2 octaves. should be ratio of 4
                   "701.955\n" //P5. should be ratio of 3/2
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE(m.loadSclFile(tmpfilePath));
            REQUIRE(m.scale.notes.at(0) == Catch::Approx(2.));
            REQUIRE(m.scale.notes.at(1) == Catch::Approx(2.));
            REQUIRE(m.scale.notes.at(2) == Catch::Approx(4.));
            REQUIRE(m.scale.notes.at(3) == Catch::Approx(3.0f/2.0f));
        }
        
        
        //TODO make this dependency part of the testing.
        //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
        SECTION("Test ignore comment lines"){ // comments start with '!'
            fputs ("!comment\n"
                   "!comment\n"
                   "!comment\n"
                   "Check cents parsing Scale\n"
                   "!comment\n"
                   "4\n"
                   "!comment\n"
                   "1200\n" //octave. should be ratio of 2
                   "!comment\n"
                   "!comment\n"
                   "1200.0\n" //octave. should be ratio of 2
                   "2400\n" //2 octaves. should be ratio of 4
                   "!comment\n"
                   "701.955\n" //P5. should be ratio of 3/2
                   "!comment\n"
                   "!comment\n"
                   "!comment\n"
                   "!comment\n"
                   ,tmpFile);
            fclose(tmpFile);
            REQUIRE(m.loadSclFile(tmpfilePath));
            REQUIRE(m.scale.description == "Check cents parsing Scale");
            REQUIRE(m.scale.notes.size() == 4);
            REQUIRE(m.scale.notes.at(0) == Catch::Approx(2.));
            REQUIRE(m.scale.notes.at(1) == Catch::Approx(2.));
            REQUIRE(m.scale.notes.at(2) == Catch::Approx(4.));
            REQUIRE(m.scale.notes.at(3) == Catch::Approx(3.0f/2.0f));
            
            //TODO make this dependency part of the testing
            //Note: this scale is the same as "Check cents parsing", and is dependant on that passing
            SECTION("Test ignore inline comments"){ //
                fputs ("Check cents parsing Scale!comment\n"
                       "4!comment\n"
                       "1200!comment\n" //octave. should be ratio of 2
                       "1200.0              !comment\n" //octave. should be ratio of 2
                       "2400!comment\n" //2 octaves. should be ratio of 4
                       "701.955 !comment\n" //P5. should be ratio of 3/2
                       ,tmpFile);
                fclose(tmpFile);
                REQUIRE(m.loadSclFile(tmpfilePath));
                REQUIRE(m.scale.description == "Check cents parsing Scale");
                REQUIRE(m.scale.notes.size() == 4);
                REQUIRE(m.scale.notes.at(0) == Catch::Approx(2.));
                REQUIRE(m.scale.notes.at(1) == Catch::Approx(2.));
                REQUIRE(m.scale.notes.at(2) == Catch::Approx(4.));
                REQUIRE(m.scale.notes.at(3) == Catch::Approx(3.0f/2.0f));
            }
        }
    }
}
