#include "RigidBodyPhysicsComponent.hpp"

// glm::mat4 RigidBodyPhysicsComponent::getTransformAsMat4() const {
//     btTransform trans;

//     // If the rigid body has a motion state, use its transform, else the defaualt world transform
//     // if( collision_object->getMotionState() ) {
//     //     collision_object->getMotionState()->getWorldTransform(trans);
//     // }
//     // else {
//         trans = collision_object->getWorldTransform();
//     // }
    
//     // Get the transform values as a GL matrix and make it into a glm::mat4
//     btScalar trans_values[16];
//     trans.getOpenGLMatrix( &trans_values[0] ); //&glm::value_ptr(mat4_trans)[0]
//     glm::mat4 mat4_trans = glm::make_mat4( trans_values );

//     return mat4_trans;
// }

void RigidBodyPhysicsComponent::getTransformationData( glm::vec3 parent_scale, glm::mat4 parent_bullet_transform, glm::vec3 &trans_out, glm::quat &rot_out ) const {
    // Get the object's transform
    // The commented block is bullet's recommended way of doing it
    // However, using motion state seems to create some time variations between runs
    btTransform trans;
    // if( collision_object->getMotionState() ) {
    //     collision_object->getMotionState()->getWorldTransform(trans);
    // }
    // else {
        trans = collision_object->getWorldTransform();
    // }

    // Multiply by the inverse parent being passed in
    btTransform bt_parent;
    bt_parent.setFromOpenGLMatrix( &glm::value_ptr(parent_bullet_transform)[0] );
    btTransform bt_local = bt_parent.inverse() * trans;

    // Translate the values to glm vectors and quaternions and pass them out
    btVector3 translation = bt_local.getOrigin();
    btQuaternion rotation = bt_local.getRotation();

    // Multiply the translation component-wise by the inverse of the parent scale to transform it to the object's local space
    trans_out = glm::vec3( (float)translation.getX(), (float)translation.getY(), (float)translation.getZ() ) / parent_scale;
    rot_out = glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
}

// Set the transformation data from the game object's data
void RigidBodyPhysicsComponent::setTransformationData( glm::mat4 object_bullet_transform ) {
    // Create the new transform for bullet
    btTransform bt_descaled;
    bt_descaled.setFromOpenGLMatrix( &glm::value_ptr(object_bullet_transform)[0] );

    // Set the transform for Bullet
    collision_object->setWorldTransform( bt_descaled );
    if( collision_object->getMotionState() ) {
       collision_object->getMotionState()->setWorldTransform( bt_descaled );
    }
}
