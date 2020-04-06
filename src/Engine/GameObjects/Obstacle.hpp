#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

// #include <glm/gtx/string_cast.hpp>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"
#include "../Systems/Physics/RigidBodyFactory.hpp"

class Obstacle: public GameObject {
private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

public:
    Obstacle();
    Obstacle( std::string id, Mesh *mesh_in, RigidBodyPhysicsComponent *physics_in );
    ~Obstacle();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const { return physics; }

    // Update any necessary data
    void updateTransformFromPhysics( glm::mat4 parent_transform );
    void setBulletTransforms() const;
};

#endif