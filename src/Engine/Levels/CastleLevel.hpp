#ifndef CASTLELEVEL_H
#define CASTLELEVEL_H

#include "Level.hpp"

#define CASTLE_LEVEL_NAME "Castle"

class CastleLevel : public Level {
  private:
  public:
    CastleLevel();

    void populateLevel() override;
};

#endif
