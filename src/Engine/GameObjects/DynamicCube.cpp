#include "DynamicCube.hpp"

DynamicCube::DynamicCube( float scale ): GameObject("DynamicCube") {
    mesh = MeshFactory::createBasicMesh(std::string("Level"));
    physics = RigidBodyFactory::createCubeComponent( identifier, scale, pow( scale, 3 ) );
}

DynamicCube::~DynamicCube() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Get the transform from the physics component 
void DynamicCube::updateTransformFromPhysics( glm::mat4 parent_transform ) {
    if( physics ) {
        // Calculate the local transform the parent and physics component
        glm::mat4 local_transform = glm::inverse(parent_transform) * physics->getTransformAsMat4();
        btTransform bt_local_transform;
        bt_local_transform.setFromOpenGLMatrix( &glm::value_ptr(local_transform)[0] );

        // Get the local rotation
        btQuaternion bt_local_rotation = bt_local_transform.getRotation();
        glm::quat new_local_rotation( bt_local_rotation.getW(), bt_local_rotation.getX(), bt_local_rotation.getY(), bt_local_rotation.getZ() );

        // Get the local translation
        btVector3 bt_local_trans = bt_local_transform.getOrigin();
        glm::vec3 new_local_trans( bt_local_trans.getX(), bt_local_trans.getY(), bt_local_trans.getZ() );

        // Set the transform
        setTransform( getScale(), new_local_rotation, new_local_trans );
    }

    // Call super method to step through children
    GameObject::updateTransformFromPhysics( world_transform );
}

// Update the transforms of the physics component just before the physics step
void DynamicCube::setBulletTransforms() const {
    if( physics ) {
        // Get an unscaled transform for bullet
        glm::mat4 to_bullet = glm::translate( glm::mat4(1.f), translation ) * glm::toMat4( rotation );

        // Convert the unscaled transform to a btTransform and pass it to bullet
        btTransform bt_in;
        bt_in.setFromOpenGLMatrix( &glm::value_ptr(to_bullet)[0] );
        btRigidBody * obj = (btRigidBody *)physics->getCollisionObject();
        
        // obj->getMotionState()->setWorldTransform( bt_in );
        obj->setWorldTransform(bt_in);

        // Call the super method to step through children
        GameObject::setBulletTransforms();
    }
}
