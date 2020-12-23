#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

// A simple wrapper to control whether to use boost or std::filesystem
#if __APPLE__
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
namespace fswrapper {
inline bool is_dir(fs::directory_entry ent) { return is_directory(ent); }
}; // namespace fswrapper

#else
#include <filesystem>
namespace fs = std::filesystem;
namespace fswrapper {
inline bool is_dir(fs::directory_entry ent) { return ent.is_directory(); }
}; // namespace fswrapper
#endif

class Util {
private:
    Util();
    static bool debug_print;

public:
    static void enableDebug() { debug_print = true; }
    static bool isDebugEnabled() { return debug_print; }

    // Remove special characters that will cause errors trying to create a file or directory
    static std::string sanitizeString(std::string directory);

    // Create a directory
    static void createFolder(std::string directory);
};

#endif