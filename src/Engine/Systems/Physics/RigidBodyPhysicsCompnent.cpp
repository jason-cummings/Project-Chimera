#include "RigidBodyPhysicsComponent.hpp"

glm::mat4 RigidBodyPhysicsComponent::getTransformAsMat4() const {
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

// void RigidBodyPhysicsComponent::getTransformationData( glm::vec3 &trans_out, glm::quat &rot_out ) const {
//     btTransform trans;
//     if( collision_object->getMotionState() ) {
//         collision_object->getMotionState()->getWorldTransform(trans);
//     }
//     else {
//         trans = collision_object->getWorldTransform();
//     }

//     btVector3 translation = trans.getOrigin();
//     btQuaternion rotation = trans.getRotation();
//     trans_out = glm::vec3( (float)translation.getX(), (float)translation.getY(), (float)translation.getZ() );
//     rot_out = glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
// }
