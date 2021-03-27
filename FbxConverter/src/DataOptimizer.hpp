#ifndef DATA_OPTIMIZER_H
#define DATA_OPTIMIZER_H

#include <map>
#include <string>

#include "Util.hpp"

// Purpose:
// This class is used to optimize object processing and usage. While interacting with the FBX SDK, a pointer to an object can be recieved
// several times during processing. for example, multiple nodes may use the same mesh object. in this case, the DataOptimizer class keeps
// track of the mesh objects that have already been processed, so that the mesh is only processed once and only exported once. This allows
// performance optimization in this program and optimizes CPU memory and GPU memory in Project Chimera

// Note: this class is specifically intended for use while exporting objects to their own directories. Given a base directory, given objects
// should have their own subdirectory with the objects "index" (determined by this class) as the folder name. For example, in a materials folder,
// there would be 5 materials, with folder names 1,2,3,4,5. This ensures that there aren't duplicate folder names. If names are important, they
// can be stored in a file in the subdirectories
class DataOptimizer {
private:
    std::map<void *, int> indexes; // maps pointers to indexes
    int mostRecentlyAddedIndex = 0;

    std::string baseDirectory;

public:
    DataOptimizer(std::string baseDirectoryIn) {
        baseDirectory = baseDirectoryIn;
    }

    DataOptimizer() {
        baseDirectory = ".";
    }

    // check if this object has already been processed
    bool checkExists(void *pointer) {
        return indexes.find(pointer) != indexes.end();
    }

    // get index of object given it has already been processed
    int getIndex(void *pointer) {
        return indexes[pointer];
    }

    // returns index for this data set and creates a directory to store this data
    int addData(void *pointer) {
        mostRecentlyAddedIndex++;
        std::cout << baseDirectory << ": " << mostRecentlyAddedIndex << std::endl;
        indexes[pointer] = mostRecentlyAddedIndex;
        Util::createFolder(baseDirectory + std::string("/") + std::to_string(mostRecentlyAddedIndex));
        return mostRecentlyAddedIndex;
    }

    std::string getDataDirectory(int index) {
        return baseDirectory + std::string("/") + std::to_string(index);
    }
};

#endif