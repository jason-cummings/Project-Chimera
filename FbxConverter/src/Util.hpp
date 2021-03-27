#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

class Util {
private:
    Util();

public:
    // Remove special characters that will cause errors trying to create a file or directory
    static std::string sanitizeString(std::string directory);

    // Create a directory
    static void createFolder(std::string directory);
};

#endif