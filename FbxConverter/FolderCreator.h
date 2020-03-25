#ifndef FOLDER_CREATOR_H
#define FOLDER_CREATOR_H

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void createFolder(std::string directory) {
	if(mkdir(directory.c_str(),0777) != 0) {
		std::cout << "Error creating directory: " << directory << std::endl;
	}
}

#endif