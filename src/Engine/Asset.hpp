// Simple class to read in a file under the ./src/Assets/ folder
// Uses the wherami library to ensure that the location from which the executable is run does not matter

#ifndef ASSET_H
#define ASSET_H

#include <whereami.h>
#include <iostream>
#include <string>
#include <fstream>

class Asset {
private:
    char *buffer;
    int n_bytes;
    bool readInAsset( std::string fpath );

public:
    Asset( std::string fname );
    ~Asset();
    char * getBuffer() { return buffer; }
    int getBytes() { return n_bytes; }

    // Use if you need to maintain the buffer data after the Asset instance has been destroyed
    char * copyBuffer();
};

#endif