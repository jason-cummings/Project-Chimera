#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

class Util {
private:
    Util();
    static bool debug_print;

public:
    static void enableDebug() { debug_print = true; }
    static bool isDebugEnabled() { return debug_print; }
};

#define DEBUG(x) if (Util::isDebugEnabled()) std::cout << x << std::endl;
#define ERROR(x) std::cerr << x << std::endl;

#endif