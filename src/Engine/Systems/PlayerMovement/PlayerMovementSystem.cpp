#include "PlayerMovementSystem.hpp"

PlayerMovementSystem::PlayerMovementSystem( PhysicsSystem *physics_in, Player* playerptr ): physics_system(physics_in), player(playerptr), camera(nullptr) {
    player_body = player->getPhysicsComponent()->getCollisionObject();
    
    // Restrict any player hitbox rotation to only be around the y axis
    player_body->setAngularFactor( btVector3(0.f, 1.f, 0.f) );
    
    // Prevent the player from disabling itself
    player_body->setActivationState( DISABLE_DEACTIVATION );

    // Assume off ground by default
    on_ground = false;

    jump_cool_down = 0;
}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::movePlayer( bool f, bool b, bool r, bool l, bool space, bool shift, double dt ) {
    // Ensure player_body has an up to date collision object
    player_body = player->getPhysicsComponent()->getCollisionObject();

    testOnGround();
    if( on_ground && jump_cool_down > 0 ) jump_cool_down -= (float)dt;

    // Determine the input movement directions
    int fb = int(b-f); // forward/backward
    int rl = int(r-l); // right/left

    // Create a normal vector in the direction the player is moving relative to the camera
    float th = camera == nullptr ? 0.f : camera->getTh();
    glm::vec3 player_move_dir = glm::vec3(
        fb * sin(th) + rl * cos(th),
        0.f,
        fb * cos(th) + rl * -sin(th)
    );
    if( glm::length( player_move_dir ) > 0 ) {
        player_move_dir = glm::normalize( player_move_dir );
    }

    // Set the linear velocity to the player body in the desired direction of movement
    glm::vec3 movement = player_move_dir * MOVE_SPEED;
    btVector3 current_velocity = player_body->getLinearVelocity();
    player_body->setLinearVelocity( btVector3( movement.x, current_velocity.getY(), movement.z ) );

    if( on_ground && jump_cool_down <= 0 && space ) {
        // Jumping time
        btVector3 current_player_velocity = player_body->getLinearVelocity();
        current_player_velocity.setY(0.f);
        player_body->setLinearVelocity( current_player_velocity );
        player_body->applyCentralImpulse( btVector3(0.f, JUMP_IMPULSE_VALUE, 0.f) );
        on_ground = false;
        jump_cool_down = JUMP_COOLDOWN_TIME;
    }
    // if( shift ) {
    //     // Jumping time
    //     player_body->applyCentralForce( btVector3(0.f, -10000.f, 0.f) );
    // }

}

void PlayerMovementSystem::flyPlayer( int ad, int ss, int ws, double dt ){
    float th = camera == nullptr ? 0.f : camera->getTh();
    float movex = MOVE_SPEED * (ws * sin(th) + ad * cos(th));
    float movey = MOVE_SPEED * (float)ss;
    float movez = MOVE_SPEED * (ws * cos(th) + ad * -sin(th));
    
    glm::vec3 movement = glm::vec3( movex, movey, movez ) * (float)dt;

    //Moves the player with the given inputs.
    player->setTranslation(  player->getTranslation() + movement );
}

void PlayerMovementSystem::testOnGround() {
    // Perform the ray test to detect ground collisions
    glm::vec3 player_pos = glm::vec3( player->getWorldTransform()[3] );
    btVector3 ray_from( player_pos.x, btScalar(player_pos.y+.1), player_pos.z );
    btVector3 ray_to( player_pos.x, -5000.f, player_pos.z );
    btCollisionWorld::ClosestRayResultCallback callback( ray_from, ray_to );
    physics_system->closestRayCast( ray_from, ray_to, callback );

    // Now 
    on_ground = false;
    if( callback.hasHit() ) {
        // std::cout << "Callback results:" << std::endl;
        // std::cout << "    Pos:  " << callback.m_hitPointWorld.getX() << ", " << callback.m_hitPointWorld.getY() << ", " << callback.m_hitPointWorld.getZ() << std::endl;
        // std::cout << "    Norm: " << callback.m_hitNormalWorld.getX() << ", " << callback.m_hitNormalWorld.getY() << ", " << callback.m_hitNormalWorld.getZ() << std::endl;

        btVector3 bt_player_pos( player_pos.x, player_pos.y, player_pos.z );
        float dist_to_ground = (float)bt_player_pos.distance( callback.m_hitPointWorld );
        if( dist_to_ground <= GROUND_DISTANCE_THRESHOLD ) {
            on_ground = true;
        }
    }
}
