#include "Obstacle.hpp"

Obstacle::Obstacle(): GameObject(1) {
    mesh = MeshFactory::createBasicMesh("Level");
    physics = RigidBodyFactory::createCube( identifier, 1.f, 1.f );
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
        // std::cout << glm::to_string(physics->getTransformAsMat4()) << std::endl;
        setTransform( local_transform ); 

        // Call super method to step through children
        GameObject::updateTransformFromPhysics( world_transform );
    }
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

        // Call the super method to step through children
        GameObject::setBulletTransforms();
    }
}
