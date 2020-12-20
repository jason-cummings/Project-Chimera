#ifndef FOLDER_CREATOR_H
#define FOLDER_CREATOR_H

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "FilesystemWrapper.hpp"

std::string sanitizeString(std::string directory);
void createFolder(std::string directory);

#endif