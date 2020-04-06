#include "Asset.hpp"

Asset::Asset( std::string fname ) {
    // Get the path to the executable, then add the file name
    std::string fpath = assetPath().string() + std::string(fname);

    // Read in the asset with the full file path
    if( !readInAsset( fpath ) ) {
        std::cerr << "Could not read asset " << fname << std::endl;
        buffer = nullptr;
    }
}

Asset::Asset( fs::path fpath ) {
    // Read in the asset with the full file path
    if( !readInAsset( fpath.string() ) ) {
        std::cerr << "Could not read asset " << fpath.string() << std::endl;
        buffer = nullptr;
    }
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
    n_bytes = file.tellg();
    file.seekg( 0, std::ios::beg );
    buffer = new char[n_bytes/sizeof(char)+1];
    file.read( buffer, n_bytes );
    file.close();
    buffer[n_bytes] = '\0';

    return true;
}

// Use if you need to maintain the buffer data after the Asset instance has been destroyed
char * Asset::copyBuffer() {
    char *retbuffer = (char *)calloc( n_bytes+1, sizeof(char) );
    for( int i=0; i<n_bytes; i++ ) {
        retbuffer[i] = buffer[i];
    }
    return retbuffer;
}
