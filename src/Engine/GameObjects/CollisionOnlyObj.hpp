#ifndef COLLISIONONLYOBJ_HPP
#define COLLISIONONLYOBJ_HPP

#include "../GameObject.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"

class CollisionOnlyObj : public GameObject {
private:
    RigidBodyPhysicsComponent *physics;

public:
    CollisionOnlyObj(std::string id, RigidBodyPhysicsComponent *physics_in);
    ~CollisionOnlyObj();

    bool hasPhysicsComponent() const override { return physics != nullptr; }
    RigidBodyPhysicsComponent *getPhysicsComponent() const override { return physics; }

    // Update any necessary data
    void setBulletTransforms() const override;
};

#endif
