#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"

class Obstacle: public GameObject {
private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

public:
    Obstacle( std::string id, Mesh *mesh_in, RigidBodyPhysicsComponent *physics_in );
    ~Obstacle();

    bool hasMesh() const override { return true; }
    Mesh * getMesh() const override { return mesh; }
    bool hasPhysicsComponent() const override { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const override { return physics; }

    // Update any necessary data
    void setBulletTransforms() const override;
};

#endif