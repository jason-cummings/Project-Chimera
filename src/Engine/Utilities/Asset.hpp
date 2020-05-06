// Simple class to read in a file under the ./src/Assets/ folder
// Uses the wherami library to ensure that the location from which the executable is run does not matter

#ifndef ASSET_H
#define ASSET_H

#include <iostream>
#include <string>
#include <fstream>

#include "WAIWrapper.hpp"
#include "FilesystemWrapper.hpp"

class Asset {
private:
    char *buffer;
    int n_bytes;
    bool read_success;
    
    bool readInAsset( std::string fpath );

public:
    // Use to read in from the Assets folder
    Asset( std::string fname );

    // Use to read in from an absolute path
    Asset( fs::path fpath );
    
    ~Asset();
    inline char * getBuffer() const { return buffer; }
    inline int getBytes() const { return n_bytes; }

    // Use if you need to maintain the buffer data after the Asset instance has been destroyed
    char * copyBuffer();

    inline bool success() const { return read_success; }

    static fs::path assetPath() { return fs::path(WAIWrapper::getExecutablePath() + "/Assets/"); };
};

#endif