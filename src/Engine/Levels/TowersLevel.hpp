#ifndef TOWERSLEVEL_H
#define TOWERSLEVEL_H

#include "Level.hpp"

class TowersLevel: public Level {
private:

public:
    TowersLevel();
    
    void populateLevel() override;
};

#endif
