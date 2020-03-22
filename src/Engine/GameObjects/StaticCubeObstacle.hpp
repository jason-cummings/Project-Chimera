#ifndef STATICCUBEOBSTACLE_HPP
#define STATICCUBEOBSTACLE_HPP

#include <GL/glew.h>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"
#include "../Systems/Physics/PhysicsComponent.hpp"

class StaticCubeObstacle: public GameObject {
private:
    Mesh *mesh;
    PhysicsComponent *physics;

public:
    StaticCubeObstacle();
    ~StaticCubeObstacle();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    PhysicsComponent * getPhysicsComponent() const { return physics; }

    // Update any necessary data
    // void update();
};

#endif