#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace testUtility {
    /* Loads a scale into m using a sclFile contained in a the string sclFile
     */
    bool loadScale(MidiProcessor &m, std::string sclFile){
        //Make temporary file path
        char tmpfilePath[80]= "/tmp/temp.XXXX";
        mkstemp(tmpfilePath);
        unlink(tmpfilePath);
        FILE* tmpFile = fopen(tmpfilePath, "w");
        
        fputs(sclFile.c_str(), tmpFile);
        fclose(tmpFile);

        bool output = m.loadSclFile(tmpfilePath);
        
        return output;
    }

/* Loads a scale into m using a sclFile contained in a the string sclFile
 */
bool loadMapping(MidiProcessor &m, std::string sclFile){
    //Make temporary file path
    char tmpfilePath[80]= "/tmp/temp.XXXX";
    mkstemp(tmpfilePath);
    unlink(tmpfilePath);
    FILE* tmpFile = fopen(tmpfilePath, "w");
    
    fputs(sclFile.c_str(), tmpFile);
    fclose(tmpFile);

    bool output = m.loadKbmFile(tmpfilePath);
    
    return output;
}

};

