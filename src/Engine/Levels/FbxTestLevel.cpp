#include "FbxTestLevel.hpp"

#include "../GameObjects/DynamicCube.hpp"

FbxTestLevel::FbxTestLevel() : Level(FBX_TEST_LEVEL_NAME) {}

void FbxTestLevel::populateLevel() {
    skybox = SkyboxFactory::getSkybox("Skyboxes/5Degrees");
    spawn_translation = glm::vec3(0.f, 0.f, 0.f);
    spawn_rotation = glm::quat(glm::vec3(0.f, glm::radians(-90.f), 0.f));

    DirectionalLight *sun = new DirectionalLight("sun");
    sun->setTransform(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(.707f, .3f, -.707f));
    sun->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    sun->setDiffuse(glm::vec3(0.5f, 0.3f, 0.2f));
    sun->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
    directional_lights.push_back(sun);
}
