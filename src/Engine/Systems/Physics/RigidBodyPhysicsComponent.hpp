/**
 * Based on current understanding, this class should only ever act as a data storage object
 * All physics handling should be done through PhysicsSystem
 */

#ifndef RIGIDBODYPHYSICSCOMPONENT_H
#define RIGIDBODYPHYSICSCOMPONENT_H

#include <btBulletDynamicsCommon.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

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
    // SHOULD NO LONGER BE NECESSARY
    // glm::mat4 getTransformAsMat4() const;

    // Get the rotation and translation as glm objects, and pass them to the corresponding output parameters
    void getTransformationData( glm::vec3 parent_scale, glm::mat4 parent_bullet_transform, glm::vec3 &trans_out, glm::quat &rot_out ) const;

    // Set the transformation data from the game object's data
    void setTransformationData( glm::mat4 object_bullet_transform );
};

#endif