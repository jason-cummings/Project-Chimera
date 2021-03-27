#ifndef __FBXDIRECTORYPROCESSOR_H__
#define __FBXDIRECTORYPROCESSOR_H__

#include <string>

#include "FilesystemWrapper.hpp"

class FbxDirectoryProcessor {
private:
    std::string directoryName;

public:
    FbxDirectoryProcessor(std::string dirName);
};

#endif // __FBXDIRECTORYPROCESSOR_H__
