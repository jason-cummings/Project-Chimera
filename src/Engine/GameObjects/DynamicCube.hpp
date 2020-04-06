#ifndef DYNAMICCUBE_HPP
#define DYNAMICCUBE_HPP

// #include <glm/gtx/string_cast.hpp>
#include <math.h>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"
#include "../Systems/Physics/RigidBodyFactory.hpp"

class DynamicCube: public GameObject {
private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

public:
    DynamicCube( float scale );
    ~DynamicCube();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const { return physics; }

    // Update any necessary data
    void updateTransformFromPhysics( glm::mat4 parent_transform );
    void setBulletTransforms() const;
};

#endif