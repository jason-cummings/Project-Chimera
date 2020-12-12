#include "Obstacle.hpp"

// #include <glm/gtx/string_cast.hpp>

#include "../Systems/Physics/RigidBodyFactory.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"

Obstacle::Obstacle(std::string id, Mesh *mesh_in, RigidBodyPhysicsComponent *physics_in) : GameObject(id) {
    mesh = mesh_in;
    physics = physics_in;
}

Obstacle::~Obstacle() {
    // if( mesh ) delete mesh;
    if (physics) delete physics;
}

// Update the transforms of the physics component just before the physics step
void Obstacle::setBulletTransforms() const {
    if (physics) {
        physics->setTransformationData(bullet_world_transform);
    }

    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
