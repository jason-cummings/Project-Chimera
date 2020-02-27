// Simple class to read in a file under the ./src/Assets/ folder
// Uses the wherami library to ensure that the location from which the executable is run does not matter

#ifndef ASSET_H
#define ASSET_H

#include <whereami.h>
#include <iostream>

class Asset {
private:
    char *buffer;
    bool readInAsset( const char *fpath );

public:
    Asset( const char *fname );
    ~Asset();
    char * getBuffer();
};

#endif