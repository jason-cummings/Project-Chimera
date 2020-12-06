#ifndef DYNAMICCUBE_HPP
#define DYNAMICCUBE_HPP

#include "../GameObject.hpp"
#include "../Systems/Physics/RigidBodyPhysicsComponent.hpp"
#include "../Systems/Rendering/Mesh.hpp"

class DynamicCube : public GameObject {
  private:
    Mesh *mesh;
    RigidBodyPhysicsComponent *physics;

    static int num_cubes;

  public:
    DynamicCube(float scale);
    ~DynamicCube();

    inline bool hasRenderable() const override { return true; }
    inline Renderable *getRenderable() const override { return (Renderable *)mesh; }
    inline bool hasPhysicsComponent() const override { return physics != nullptr; }
    inline RigidBodyPhysicsComponent *getPhysicsComponent() const override { return physics; }

    // Update any necessary data
    void updateTransformFromPhysics(glm::vec3 parent_scale, glm::mat4 parent_bullet_transfrom) override;
    void setBulletTransforms() const override;
};

#endif