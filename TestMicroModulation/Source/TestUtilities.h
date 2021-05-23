#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace testUtility {
    /* Loads a scale into m using a sclFile contained in a the string sclFile
     */
    bool loadScale(MidiProcessor &m, std::string sclFile){
        //Make temporary file path
        char tmpfilePath[80]= "/tmp/tempscale.XXXXXX"; //don't know what 80 size is about. took it from stackexchange
        mkstemp(tmpfilePath);
        //write sclFile to tmpFile
        FILE* tmpFile = fopen(tmpfilePath, "w");
        fputs(sclFile.c_str(), tmpFile);
        fclose(tmpFile);
        
        bool output = m.loadSclFile(tmpfilePath);
        unlink(tmpfilePath);
        return output;
    }

/* Loads a mapping into m using a sclFile contained in a the string sclFile
 */
bool loadMapping(MidiProcessor &m, std::string sclFile){
    //Make temporary file path
    char tmpfilePath[80]= "/tmp/temp.XXXXXX";
    mkstemp(tmpfilePath);
    FILE* tmpFile = fopen(tmpfilePath, "w");
    
    fputs(sclFile.c_str(), tmpFile);
    fclose(tmpFile);
    
    
    
    
    
    unlink(tmpfilePath);


    //bool output = m.loadKbmFile(tmpfilePath);
    
   // return output;
    return false;
}

};

