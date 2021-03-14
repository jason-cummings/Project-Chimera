#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>

#include "Level.hpp"

class LevelManager {
private:
    LevelManager() {}
    LevelManager(const LevelManager &) = delete;
    LevelManager &operator=(const LevelManager &) = delete;

public:
    static Level *getLevel(std::string name);
};

#endif
