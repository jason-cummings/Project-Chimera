#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>

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

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    PhysicsComponent * getPhysicsComponent() const { return physics; }

    // Update any necessary data
    void update();
    void updateTransformFromPhysics( glm::mat4 parent_transform );
    void setBulletTransforms() const;
};

#endif