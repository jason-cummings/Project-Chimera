#ifndef RIGIDBODYFACTORY_H
#define RIGIDBODYFACTORY_H

#include <map>
#include <iostream>
#include <btBulletDynamicsCommon.h>
#include "RigidBodyPhysicsComponent.hpp"

class RigidBodyFactory {
private:
    // Store created collision shapes for reuse whenever possible
    static std::map<std::string, btCollisionShape*> collision_shapes;

    // Return the collision shape with name if it exists, else nullptr
    static btCollisionShape* findCollisionShape( std::string name );

public:
    static RigidBodyPhysicsComponent * createCube( int id, float side_length, float mass );
};

#endif