#include "Asset.hpp"

Asset::Asset( std::string fname ) {
    // Get the path to the executable, then add the file name
    int length = wai_getExecutablePath( NULL, 0, NULL );
    char *loc = (char *)calloc( length+1, sizeof(char) );
    wai_getExecutablePath( loc, length, &length );
    loc[length] = '\0';
    std::string fpath = std::string(loc) + "/Assets/" + std::string(fname);

    // Read in the asset with the full file path
    if( !readInAsset( fpath ) ) {
        std::cerr << "Could not read asset " << fname << std::endl;
        buffer = nullptr;
    }
    
    free( loc );
}

Asset::~Asset() {
    delete buffer;
}

bool Asset::readInAsset( std::string fpath ) {
    std::ifstream file;
    file.open( fpath, std::ios::in | std::ios::binary );
    if( !file.is_open() ) {
        std::cout << "Could not open file " << fpath << std::endl;
        return false;
    }

    file.seekg( 0, std::ios::end );
    length = file.tellg();
    file.seekg( 0, std::ios::beg );
    buffer = new char[length+1];
    file.read( buffer, length );
    file.close();
    buffer[length] = '\0';

    return true;
}

char * Asset::getBuffer() {
    return buffer;
}
