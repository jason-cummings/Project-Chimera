/**
 * Based on current understanding, this class should only ever act as a data storage object
 * All physics handling should be done through PhysicsSystem
 */

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <btBulletDynamicsCommon.h>

class PhysicsComponent {
private:
    // ID of the associated GameObject
    int id;

    // Keep track of the shape and object
    // Shape should be in common between instances of PhysicsComponent whenever possible
    btCollisionShape *collision_shape;
    btCollisionObject *collision_object;

public:
    PhysicsComponent( int obj_id, btCollisionShape *col_sh, btCollisionObject *col_obj ):
        id(obj_id), collision_shape(col_sh), collision_object(col_obj) {}
    ~PhysicsComponent() {}

    // Getters
    int getID() { return id; }
    btCollisionShape * getCollisionShape() { return collision_shape; }
    btCollisionObject * getCollisionObject() { return collision_object; }
};

#endif