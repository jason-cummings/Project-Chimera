#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <GL/glew.h>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"
#include "../Systems/Physics/PhysicsComponent.hpp"

class Obstacle: public GameObject {
private:
    Mesh *mesh;
    PhysicsComponent *physics;

public:
    Obstacle();
    ~Obstacle();

    bool hasMesh() { return true; }
    Mesh * getMesh() { return mesh; }
    bool hasPhysicsComponent() { return physics != nullptr; }
    PhysicsComponent * getPhysicsComponent() { return physics; }
};

#endif