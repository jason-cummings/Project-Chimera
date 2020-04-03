#include "Obstacle.hpp"

Obstacle::Obstacle(): GameObject("obstacle") {
    mesh = MeshFactory::createBasicMesh(std::string("Level"));
    physics = RigidBodyFactory::createCube( identifier, 1.f, 1.f );
}

Obstacle::Obstacle( std::string id, Mesh *mesh_in, RigidBodyPhysicsComponent *physics_in ): GameObject(id) {
    mesh = mesh_in;
    physics = physics_in;
}

Obstacle::~Obstacle() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Get the transform from the physics component 
void Obstacle::updateTransformFromPhysics( glm::mat4 parent_transform ) {
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
void Obstacle::setBulletTransforms() const {
    if( physics ) {
        // Convert the worldspace transform and pass it to bullet
        btTransform bt_in;
        bt_in.setFromOpenGLMatrix( &glm::value_ptr(world_transform)[0] );
        btRigidBody * obj = (btRigidBody *)physics->getCollisionObject();
        
        // obj->getMotionState()->setWorldTransform( bt_in );
        obj->setWorldTransform(bt_in);
    }
    
    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
