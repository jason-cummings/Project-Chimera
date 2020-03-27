/**
 * Based on current understanding, this class should only ever act as a data storage object
 * All physics handling should be done through PhysicsSystem
 */

#ifndef RIGIDBODYPHYSICSCOMPONENT_H
#define RIGIDBODYPHYSICSCOMPONENT_H

#include <btBulletDynamicsCommon.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class RigidBodyPhysicsComponent {
private:
    // ID of the associated GameObject
    std::string id;

    // Keep track of the shape and object
    // Shape should be in common between instances of PhysicsComponent whenever possible
    btCollisionShape *collision_shape;
    btRigidBody *collision_object;

public:
    RigidBodyPhysicsComponent( std::string obj_id, btCollisionShape *col_sh, btRigidBody *col_obj ):
        id(obj_id), collision_shape(col_sh), collision_object(col_obj) {}
    ~RigidBodyPhysicsComponent() {}

    // Getters
    std::string getID() const { return id; }
    btCollisionShape * getCollisionShape() const { return collision_shape; }
    btRigidBody * getCollisionObject() const { return collision_object; }

    // Return the collision object's transform as a glm::mat4 to sync with graphics.
    glm::mat4 getTransformAsMat4() const {
        btTransform trans;

        // If the rigid body has a motion state, use its transform, else the defaualt world transform
        if( collision_object->getMotionState() ) {
            collision_object->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = collision_object->getWorldTransform();
        }
        
        // Get the transform values as a GL matrix and make it into a glm::mat4
        btScalar trans_values[16];
        trans.getOpenGLMatrix( &trans_values[0] ); //&glm::value_ptr(mat4_trans)[0]
        glm::mat4 mat4_trans = glm::make_mat4( trans_values );

        return mat4_trans;
    }
};

#endif