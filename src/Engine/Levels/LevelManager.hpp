#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>

#include "Level.hpp"

class LevelManager {
  private:
    LevelManager() {}

  public:
    static Level *getLevel(std::string name);
};

#endif
