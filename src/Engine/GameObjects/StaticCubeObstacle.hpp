#ifndef STATICCUBEOBSTACLE_HPP
#define STATICCUBEOBSTACLE_HPP

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"
#include "../Systems/Physics/RigidBodyFactory.hpp"

class StaticCubeObstacle: public GameObject {
private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

public:
    StaticCubeObstacle();
    ~StaticCubeObstacle();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const { return physics; }

};

#endif