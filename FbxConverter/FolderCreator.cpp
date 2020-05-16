#include "FolderCreator.h"

// creates a folder given a directory name
void createFolder(std::string directory) {
	if(mkdir(directory.c_str(),0777) != 0) {
		std::cout << "Error creating directory: " << directory << std::endl;
	}
}