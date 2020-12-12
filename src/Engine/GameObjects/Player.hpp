#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./../GameObject.hpp"
#include "./../Systems/Physics/RigidBodyPhysicsComponent.hpp"
#include "./../Systems/Rendering/Mesh.hpp"

#define PLAYER_HEIGHT 1.5f
#define PLAYER_DIAMETER .5f
#define PLAYER_MASS 100.f

class Player : public GameObject {
  private:
    SkinnedMesh *skinned_mesh;
    RigidBodyPhysicsComponent *physics;

  public:
    Player(std::string identifier, SkinnedMesh *skinned_mesh_in);
    ~Player();

    inline bool hasRenderable() const override { return true; }
    inline Renderable *getRenderable() const override { return (Renderable *)skinned_mesh; }
    inline bool hasPhysicsComponent() const override { return true; }
    inline RigidBodyPhysicsComponent *getPhysicsComponent() const override { return physics; }

    // Update any necessary data
    void updateTransformFromPhysics(glm::vec3 parent_scale, glm::mat4 parent_bullet_transfrom) override;
    void setBulletTransforms() const override;
};

#endif