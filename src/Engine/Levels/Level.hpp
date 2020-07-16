#ifndef LEVEL_H
#define LEVEL_H

#include <string>

#include "../LevelLoader.hpp"
#include "../GameObject.hpp"
#include "../Systems/Rendering/Skybox.hpp"

#include "../Systems/Animation/AnimationSystem.hpp"

class Level {
protected:
    LevelLoader *level_loader;

    GameObject *scene;
    Skybox *skybox;
    glm::vec3 goal;

public:
    Level( std::string name );
    virtual ~Level();

    virtual void populateLevel() = 0;

    inline GameObject * getScene() { return scene; }
    inline Skybox * getSkybox() { return skybox; }
    inline glm::vec3 getGoalCoordinates() { return goal; }

    void populateAnimationSystem( AnimationSystem *animation_system );
};

#endif
