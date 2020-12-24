#include "Util.hpp"

const char *str_match = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_./\\";

bool is_special_char(char c) {
    return strchr(str_match, c) == NULL;
}

std::string Util::sanitizeString(std::string directory) {
    std::string::iterator new_end = std::remove_if(directory.begin(), directory.end(), is_special_char);
    directory.resize(new_end - directory.begin());
    return directory;
}

void Util::createFolder(std::string directory) {
    std::string sanitized_directory = sanitizeString(directory);
    if (!fs::create_directory(fs::path(sanitized_directory))) {
        std::cout << "Error creating directory: " << sanitized_directory << std::endl;
    }
}
