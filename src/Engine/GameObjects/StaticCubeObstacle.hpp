#ifndef STATICCUBEOBSTACLE_HPP
#define STATICCUBEOBSTACLE_HPP

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"

class StaticCubeObstacle: public GameObject {
private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

public:
    StaticCubeObstacle( float scale );
    ~StaticCubeObstacle();

    inline bool hasRenderable() const override { return true; }
    inline Renderable * getRenderable() const override { return (Renderable *) mesh; }
    bool hasPhysicsComponent() const { return physics != nullptr; }
    RigidBodyPhysicsComponent * getPhysicsComponent() const { return physics; }

    void setBulletTransforms() const;
};

#endif