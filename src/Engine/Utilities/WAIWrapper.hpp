#ifndef WAIWRAPPER_H
#define WAIWRAPPER_H

#include <string>
#include <whereami.h>

namespace WAIWrapper {
inline std::string getExecutablePath() {
    int path_len = wai_getExecutablePath(NULL, 0, NULL);
    char *loc = (char *)calloc(path_len + 1, sizeof(char));
    wai_getExecutablePath(loc, path_len, &path_len);
    loc[path_len] = '\0';
    std::string ret(loc);
    free(loc);
    return ret;
}
}; // namespace WAIWrapper

#endif