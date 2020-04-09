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

    static int num_cubes;

public:
    DynamicCube( float scale );
    ~DynamicCube();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const { return physics; }

    // Update any necessary data
    void updateTransformFromPhysics( glm::vec3 parent_scale, glm::mat4 parent_bullet_transfrom ) override;
    void setBulletTransforms() const override;
};

#endif