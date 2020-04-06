#ifndef RIGIDBODYFACTORY_H
#define RIGIDBODYFACTORY_H

#include <map>
#include <iostream>
#include <btBulletDynamicsCommon.h>
#include "RigidBodyPhysicsComponent.hpp"
#include "../../Asset.hpp"

class RigidBodyFactory {
private:
    // Store created collision shapes for reuse whenever possible
    static std::map<std::string, btCollisionShape*> collision_shapes;

    // Return the collision shape with name if it exists, else nullptr
    static btCollisionShape * findCollisionShape( std::string name );

    static btRigidBody * createRigidBody( btCollisionShape *collision_shape, float mass );

public:
    static RigidBodyPhysicsComponent * createCube( std::string id, float side_length, float mass );
    
    static RigidBodyPhysicsComponent * createBvhTriangleMeshFromFiles( std::string id, std::string directory_name );
};

#endif