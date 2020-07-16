#ifndef CASTLELEVEL_H
#define CASTLELEVEL_H

#include "Level.hpp"

class CastleLevel: public Level {
private:

public:
    CastleLevel();
    
    void populateLevel() override;
};

#endif
