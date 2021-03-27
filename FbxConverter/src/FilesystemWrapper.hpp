#ifndef FILESYSTEMWRAPPER_H
#define FILESYSTEMWRAPPER_H

// A simple wrapper to control whether to use boost or std::filesystem
#if __APPLE__
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
namespace fswrapper {
inline bool isDir(fs::directory_entry ent) { return is_directory(ent); }
} // namespace fswrapper

#else
#include <filesystem>
namespace fs = std::filesystem;
namespace fswrapper {
inline bool isDir(fs::directory_entry ent) { return ent.is_directory(); }
} // namespace fswrapper
#endif

#endif // FILESYSTEMWRAPPER_H