#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger {
private:
    static unsigned short step;
    static bool logEnabled;
    Logger();

public:
    static void enableLog() { logEnabled = true; }

    static void log(std::string string);
    static void error(std::string string);

    static void stepIn() { step += 4; }
    static void stepUp() {
        if (step >= 4) step -= 4;
    }
};

#endif