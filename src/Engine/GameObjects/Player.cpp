#include "Player.hpp"

Player::Player(): GameObject("Player"){
    mesh = MeshFactory::createBasicMesh(std::string("Player"));
    physics = RigidBodyFactory::createCapsuleComponent( std::string("Player"), PLAYER_HEIGHT, PLAYER_DIAMETER, PLAYER_MASS );
}

Player::~Player(){
    delete mesh;
}

// Get the transform from the physics component 
void Player::updateTransformFromPhysics( glm::vec3 parent_scale, glm::mat4 parent_bullet_transfrom ) {
    // Get the updated transformation data from physics
    if( physics ) {
        glm::vec3 new_translation;
        glm::quat new_rotation;
        physics->getTransformationData( parent_scale, parent_bullet_transfrom, new_translation, new_rotation );

        physics->getCollisionObject()->setAngularVelocity( btVector3(0.f, 0.f, 0.f) );
        glm::vec3 adjusted_translation = new_translation - glm::vec3( 0.f, PLAYER_HEIGHT/2.f, 0.f );
        setTransform( scale, new_rotation, adjusted_translation );
    }

    // Call super method to step through children
    GameObject::updateTransformFromPhysics( scale, bullet_world_transform );
}

// Update the transforms of the physics component just before the physics step
void Player::setBulletTransforms() const {
    if( physics ) {
        glm::mat4 adjusted_bullet_transform = glm::translate( bullet_world_transform, glm::vec3( 0.f, PLAYER_HEIGHT/2.f, 0.f ) );
        physics->setTransformationData( adjusted_bullet_transform );
    }

    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
