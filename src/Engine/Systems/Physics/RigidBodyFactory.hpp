#ifndef RIGIDBODYFACTORY_H
#define RIGIDBODYFACTORY_H

#include <btBulletDynamicsCommon.h>
#include <map>

#include "../../Utilities/FilesystemWrapper.hpp"
#include "RigidBodyPhysicsComponent.hpp"

class RigidBodyFactory {
  private:
    // Store created collision shapes for reuse whenever possible
    static std::map<std::string, btCollisionShape *> created_collision_shapes;

    static btRigidBody *createRigidBody(btCollisionShape *collision_shape, float mass);

  public:
    // Return the collision shape with name if it exists, else nullptr
    static btCollisionShape *findCollisionShape(std::string name);

    static btBvhTriangleMeshShape *createBvhTriangleMeshFromFiles(fs::path directory);

    static RigidBodyPhysicsComponent *createCubeComponent(std::string id, float side_length, float mass);

    static RigidBodyPhysicsComponent *createCapsuleComponent(std::string id, float height, float diameter, float mass);

    static RigidBodyPhysicsComponent *createBvhTriangleMeshComponent(std::string id, btBvhTriangleMeshShape *collision_shape, glm::vec3 scale = glm::vec3(1.f));

    static RigidBodyPhysicsComponent *createComponent(std::string id, btCollisionShape *collision_shape, float mass);
};

#endif