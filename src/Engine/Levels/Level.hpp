#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "../LevelLoader.hpp"
#include "../GameObject.hpp"
#include "../Systems/Rendering/Skybox.hpp"
#include "../GameObjects/DirectionalLight.hpp"
#include "../GameObjects/PointLight.hpp"

#include "../Systems/Animation/AnimationSystem.hpp"

class Level {
protected:
    LevelLoader *level_loader;

    GameObject *scene;
    Skybox *skybox;
    glm::vec3 spawn_translation, goal;
    glm::quat spawn_rotation;

    std::vector<DirectionalLight*> directional_lights;
    std::vector<PointLight*> point_lights;

public:
    Level( std::string name );
    virtual ~Level();

    virtual void populateLevel() = 0;

    inline GameObject * getScene() { return scene; }
    inline Skybox * getSkybox() { return skybox; }
    inline glm::vec3 getGoalCoordinates() { return goal; }
    inline glm::vec3 getSpawnTranslation() { return spawn_translation; }
    inline glm::quat getSpawnRotation() { return spawn_rotation; }

    inline std::vector<DirectionalLight*> getDirectionalLights() { return directional_lights; }
    inline std::vector<PointLight*> getPointLights() { return point_lights; }

    void populateAnimationSystem( AnimationSystem *animation_system );
};

#endif
