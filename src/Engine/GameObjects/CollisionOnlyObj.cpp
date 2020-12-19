#include "CollisionOnlyObj.hpp"

CollisionOnlyObj::CollisionOnlyObj(std::string id, RigidBodyPhysicsComponent *physics_in) : GameObject(id) {
    physics = physics_in;
}

CollisionOnlyObj::~CollisionOnlyObj() {
    if (physics) delete physics;
}

// Update the transforms of the physics component just before the physics step
void CollisionOnlyObj::setBulletTransforms() const {
    if (physics) {
        physics->setTransformationData(bullet_world_transform);
    }

    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
