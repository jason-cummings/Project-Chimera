#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(std::string id) : GameObject(id) {
    for (int i = 0; i < 4; i++) {
        proj_matrices.push_back(glm::ortho(-distance_thresholds[i], distance_thresholds[i],
                                           -distance_thresholds[i], distance_thresholds[i],
                                           -100.f, 100.f));
    }
}
