#include "Util.hpp"

#include "FilesystemWrapper.hpp"

const char *strMatch = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_./\\";

bool isSpecialChar(char c) {
    return strchr(strMatch, c) == NULL;
}

std::string Util::sanitizeString(std::string directory) {
    std::string::iterator newEnd = std::remove_if(directory.begin(), directory.end(), isSpecialChar);
    directory.resize(newEnd - directory.begin());
    return directory;
}

void Util::createFolder(std::string directory) {
    std::string sanitizedDirectory = sanitizeString(directory);
    if (!fs::create_directory(fs::path(sanitizedDirectory))) {
        std::cout << "Error creating directory: " << sanitizedDirectory << std::endl;
    }
}
