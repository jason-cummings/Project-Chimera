#ifndef FBXLEVEL_HPP
#define FBXLEVEL_HPP

#include "Level.hpp"

#define FBX_TEST_LEVEL_NAME "FbxTest"

class FbxTestLevel : public Level {
  private:
  public:
    FbxTestLevel();

    void populateLevel() override;
};

#endif
