#include "LevelManager.hpp"

#include "CastleLevel.hpp"
#include "FbxTestLevel.hpp"
#include "TowersLevel.hpp"

Level *LevelManager::getLevel(std::string name) {
    if (name == TOWERS_LEVEL_NAME) {
        return new TowersLevel();
    } else if (name == CASTLE_LEVEL_NAME) {
        return new CastleLevel();
    } else if (name == FBX_TEST_LEVEL_NAME) {
        return new FbxTestLevel();
    } else {
        std::cerr << "Invalid level name " << name << std::endl;
        std::cerr << "Valid level names are: " << TOWERS_LEVEL_NAME << ", " << CASTLE_LEVEL_NAME << ", " << FBX_TEST_LEVEL_NAME << std::endl;
        return nullptr;
    }
}
