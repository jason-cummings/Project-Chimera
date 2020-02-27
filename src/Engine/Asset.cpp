#include "Asset.hpp"

Asset::Asset( const char *fname ) {
    // Get the path to the executable, then add the file name
    int length = wai_getExecutablePath( NULL, 0, NULL );
    char *loc = (char *)calloc( length+1, sizeof(char) );
    wai_getExecutablePath( loc, length, &length );
    loc[length] = '\0';
    const char * fpath = (std::string(loc) + "/Assets/" + std::string(fname)).c_str();

    // Read in the asset with the full file path
    if( !readInAsset( fpath ) ) {
        std::cerr << "Could not read asset " << fname << std::endl;
        buffer = nullptr;
    }
    
    free( loc );
}

Asset::~Asset() {
    free( buffer );
}

bool Asset::readInAsset( const char *fpath ) {

    // Attempt to open the file for reading
    FILE* file = std::fopen( fpath, "r" );
    if( file == nullptr ) {
        // Couldn't open the file
		std::cerr << "Could not open path " << fpath << std::endl;
        return false;
	}

    // Get the number of bytes in the sile and allocate a char buffer
	std::fseek( file, 0, SEEK_END );
	size_t nbytes = std::ftell( file );
	std::fseek( file, 0, SEEK_SET );
    if( (buffer = (char *)calloc(nbytes+1, sizeof(char))) == NULL ) {
        // Couldn't allocate the buffer
        std::cerr << "Unable to allocate " << nbytes+1 << " bytes for file " << fpath << std::endl;
        return false;
    }

    // Attempt to read in the file to shader_src
    if( std::fread( buffer, nbytes, 1, file ) != 1 ) {
        std::cerr << "Error reading file " << fpath << std::endl;
        free(buffer);
        return false;
    }

    // Done, close the file
    std::fclose( file );
    return true;
}

char * Asset::getBuffer() {
    return buffer;
}
