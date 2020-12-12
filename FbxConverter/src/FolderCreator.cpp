#include "FolderCreator.h"

// creates a folder given a directory name
void createFolder(std::string directory) {
    if (!fs::create_directory(fs::path(directory))) {
        std::cout << "Error creating directory: " << directory << std::endl;
    }
}
