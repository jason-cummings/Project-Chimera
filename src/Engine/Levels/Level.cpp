#include "Level.hpp"

Level::Level( std::string level_name ) {
    level_loader = LevelLoader::loadLevelFile( level_name );

    scene = level_loader->getScene();
    skybox = nullptr;
    goal = glm::vec3( 100000.f, 100000.f, 100000.f );
}

Level::~Level() {
    delete level_loader;
}

void Level::populateAnimationSystem( AnimationSystem *animation_system ) {
    for(int i = 0; i < level_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(level_loader->getAnimationStack(i));
        level_loader->getAnimationStack(i)->startAllAnimations();
    }

    if( level_loader->getJointList() != nullptr ) {
        animation_system->addJointList(level_loader->getJointList());
    }
}
