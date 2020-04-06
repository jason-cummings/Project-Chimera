#include "Obstacle.hpp"

Obstacle::Obstacle( std::string id, Mesh *mesh_in, RigidBodyPhysicsComponent *physics_in ): GameObject(id) {
    mesh = mesh_in;
    physics = physics_in;
}

Obstacle::~Obstacle() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Update the transforms of the physics component just before the physics step
void Obstacle::setBulletTransforms() const {
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
