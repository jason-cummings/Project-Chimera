#include "MeshList.hpp"

MeshList::MeshList() {
    camera_loc = glm::vec3(0.0f);
}

float MeshList::dist( GameObject * obj ) {
    glm::vec3 obj_translation = glm::vec3(obj->getWorldTransform()[3]);
    return glm::distance(camera_loc,obj_translation);
}