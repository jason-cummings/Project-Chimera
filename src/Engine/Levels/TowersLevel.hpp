#ifndef TOWERSLEVEL_H
#define TOWERSLEVEL_H

#include "Level.hpp"

#define TOWERS_LEVEL_NAME "Towers"

class TowersLevel : public Level {
  public:
    TowersLevel();

    void populateLevel() override;
};

#endif
