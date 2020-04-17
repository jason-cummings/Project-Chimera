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

    // Calculate control variables for this timestep
    testOnGround();
    if( on_ground && jump_cool_down > 0 ) jump_cool_down -= (float)dt;
    bool moving = f || b || r || l || space || shift;
    btScalar camera_th = btScalar(camera == nullptr ? 0.f : camera->getTh());

    // Get current transform information
    btVector3 current_velocity = player_body->getLinearVelocity();
    btQuaternion current_orientation = player_body->getOrientation();
    btVector3 current_position = player_body->getWorldTransform().getOrigin();

    // Get the angle and axis of the player's current orientation in the same scale as camera_th
    btScalar current_th = current_orientation.getAngle();
    btVector3 current_axis = current_orientation.getAxis();
    if( current_th > 3.1416 ) {
        current_th -= 6.2832;
    }
    if( current_axis.getY() < 0 ) {
        current_th *= -1;
    }
    
    // Get a vector of the input movements relative to the camera
    btVector3 camera_relative_move_dir = btVector3( int(f-b) * sin(camera_th) + int(l-r) * cos(camera_th), 0.f, int(f-b) * cos(camera_th) + int(l-r) * -sin(camera_th) );

    // Set friction to 0 by default and change as necessary
    player_body->setFriction(0.f);

    // Only rotate if there is a non-canceled movement input
    if( (f ^ b) || (r ^ l) ) {
        btScalar desired_angle = btVector3(0.f, 0.f, 1.f).angle( camera_relative_move_dir.normalize() );
        btScalar desired_cross = btVector3(0.f, 0.f, 1.f).cross( camera_relative_move_dir.normalize() ).getY();
        btScalar desired_th = desired_cross < 0.f ? -desired_angle : desired_angle;

        // Calculate the angle difference and bound it appropriately
        btScalar delta_th = desired_th - current_th;
        if( delta_th < -3.1416 ) {
            delta_th += 6.2832;
        }
        else if( delta_th > 3.1416 ) {
            delta_th -= 6.2832;
        }

        if( abs(delta_th) < .01 ) {
            // Stop rotating
            player_body->setAngularVelocity( btVector3(0.f, 0.f, 0.f) );
        }
        else {
            // Apply angular velocity
            btVector3 turn_axis(0.f, 1.f, 0.f);
            btScalar turn_value = btScalar(delta_th / TURN_TIMEFACTOR);
            btVector3 angular_velocity = turn_value * turn_axis;
            player_body->setAngularVelocity(angular_velocity);
        }
    }

    // Test movement scenarios
    if( (f ^ b) || (r ^ l) ) {
        // Create a normal vector in the direction the player is moving
        btVector3 player_move_dir( -sin(current_th), 0.f, -cos(current_th) );

        if( on_ground ) {
            // Set the linear velocity to the player body in the desired direction of movement
            btVector3 new_player_velocity = player_move_dir * GROUND_MOVE_SPEED;
            new_player_velocity.setY( current_velocity.getY() );
            player_body->setLinearVelocity( new_player_velocity );
            player_body->setDamping( 0.f, 0.f );
        }
        else {
            // Set the linear velocity to the player body in the desired direction of movement
            btVector3 applied_force = player_move_dir * AIR_MOVE_FORCE;
            player_body->applyCentralForce( applied_force );
            player_body->setDamping( .5f, 0.f );
        }
    }
    else {
        // NOT MOVING
        if( on_ground ) {
            // Set linear velocity to zero and set friction high to prevent sliding
            // player_body->setLinearVelocity( btVector3( 0.f, 0.f, 0.f ) );
            // player_body->setDamping( .99f, 0.f );
            player_body->setFriction(10.f);
        }
    }

    // Test for jumping conditions
    if( space && on_ground && jump_cool_down <= 0 ) {
        // Jumping time
        current_velocity.setY(0.f);
        player_body->setLinearVelocity( current_velocity );
        player_body->applyCentralImpulse( btVector3(0.f, JUMP_IMPULSE_VALUE, 0.f) );
        on_ground = false;
        jump_cool_down = JUMP_COOLDOWN_TIME;
    }
}

void PlayerMovementSystem::flyPlayer( int ad, int ss, int ws, double dt ){
    float th = camera == nullptr ? 0.f : camera->getTh();
    float movex = GROUND_MOVE_SPEED * (ws * sin(th) + ad * cos(th));
    float movey = GROUND_MOVE_SPEED * (float)ss;
    float movez = GROUND_MOVE_SPEED * (ws * cos(th) + ad * -sin(th));
    
    glm::vec3 movement = glm::vec3( movex, movey, movez ) * (float)dt;

    //Moves the player with the given inputs.
    player->setTranslation( player->getTranslation() + movement );
}

void PlayerMovementSystem::testOnGround() {
    // Perform the ray test to detect ground collisions
    glm::vec3 player_pos = glm::vec3( player->getWorldTransform()[3] );
    btVector3 ray_from( player_pos.x, btScalar(player_pos.y+PLAYER_DIAMETER/2.f), player_pos.z );

    btVector3 ray_to( player_pos.x, player_pos.y-PLAYER_DIAMETER/2.f, player_pos.z );
    btCollisionWorld::ClosestRayResultCallback callback( ray_from, ray_to );
    physics_system->closestRayCast( ray_from, ray_to, callback );

    // Assume not on ground and test if the player should be
    on_ground = false;
    if( callback.hasHit() ) {
        // std::cout << "Callback results:" << std::endl;
        // std::cout << "    Pos:  " << callback.m_hitPointWorld.getX() << ", " << callback.m_hitPointWorld.getY() << ", " << callback.m_hitPointWorld.getZ() << std::endl;
        // std::cout << "    Norm: " << callback.m_hitNormalWorld.getX() << ", " << callback.m_hitNormalWorld.getY() << ", " << callback.m_hitNormalWorld.getZ() << std::endl;

        float dist_to_collision = (float)ray_from.distance( callback.m_hitPointWorld );
        if( dist_to_collision <= GROUND_DISTANCE_THRESHOLD + PLAYER_DIAMETER/2.f ) {
            on_ground = true;
        }
    }
    if( !on_ground ) {
        // Cast 8 rays at angle RAYCAST_ANGLE to test if on the ground (approximating a cone test)
        for( int i=0; i<8; i++ ) {
            // Create the new callback object 
            float th = i * 6.2830f / 8.f;
            // std::cout << "Casting " << i << " at " << th << std::endl;
            btVector3 ray_offset( sin(RAYCAST_ANGLE)*cos(th), cos(RAYCAST_ANGLE), sin(RAYCAST_ANGLE)*sin(th) );
            ray_to = ray_from + (PLAYER_DIAMETER * ray_offset);
            btCollisionWorld::ClosestRayResultCallback angled_callback( ray_from, ray_to );
            // std::cout << "  Casting from " << ray_from.getX() << ", " << ray_from.getY() << ", " << ray_from.getZ() << std::endl;
            // std::cout << "  Casting to   " << ray_to.getX() << ", " << ray_to.getY() << ", " << ray_to.getZ() << std::endl;
            
            physics_system->closestRayCast( ray_from, ray_to, angled_callback );
            if( angled_callback.hasHit() ) {
                // std::cout << "angled_callback results:" << std::endl;
                // std::cout << "    Pos:  " << angled_callback.m_hitPointWorld.getX() << ", " << angled_callback.m_hitPointWorld.getY() << ", " << angled_callback.m_hitPointWorld.getZ() << std::endl;
                // std::cout << "    Norm: " << angled_callback.m_hitNormalWorld.getX() << ", " << angled_callback.m_hitNormalWorld.getY() << ", " << angled_callback.m_hitNormalWorld.getZ() << std::endl;

                float dist_to_collision = (float)ray_from.distance( angled_callback.m_hitPointWorld );
                if( dist_to_collision <= GROUND_DISTANCE_THRESHOLD  + PLAYER_DIAMETER/2.f ) {
                    on_ground = true;
                    break;
                }
            }
        }
    }
}
