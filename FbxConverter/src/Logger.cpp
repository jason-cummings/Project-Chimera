#include "Logger.hpp"

#define RED "\033[1;31m "
#define NC "\033[0m "

#include <sstream>

unsigned short Logger::step = 0;
bool Logger::logEnabled = false;

void Logger::log(std::string log) {
    if (logEnabled) {
        std::string stepString = std::string(step, ' ');
        std::cout << stepString << log << std::endl;
    }
}

void Logger::error(std::string error) {
    std::string stepString = std::string(step, ' ');
    std::cout << stepString << RED << "ERROR: " << NC << error << std::endl;
}
