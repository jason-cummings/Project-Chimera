/**
 * Based on current understanding, this class should only ever act as a data storage object
 * All physics handling should be done through PhysicsSystem
 */

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <btBulletDynamicsCommon.h>
#include <glm/mat4x4.hpp>

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
    int getID() const { return id; }
    btCollisionShape * getCollisionShape() const { return collision_shape; }
    btCollisionObject * getCollisionObject() const { return collision_object; }

    // Return the collision object's transform as a glm::mat4 to sync with graphics.
    glm::mat4 getTransformAsMat4() const {
        btRigidBody *body = btRigidBody::upcast(collision_object);
        btTransform trans;

        if( body->getMotionState() ) {
            body->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = collision_object->getWorldTransform();
        }
        
        btScalar trans_values[16];
        trans.getOpenGLMatrix( &trans_values[0] ); //&glm::value_ptr(mat4_trans)[0]
        glm::mat4 mat4_trans = glm::make_mat4( trans_values );

        return mat4_trans;
    }
};

#endif