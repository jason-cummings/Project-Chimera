#include "PlayerMovementSystem.hpp"

#include <glm/vec3.hpp>
#include <math.h>

#include <iostream>

#define GROUND_MOVE_SPEED 3.f    // m/s
#define GROUND_SPRINT_SPEED 6.f  // m/s
#define AIR_MOVE_FORCE 1000.f    // ?
#define JUMP_IMPULSE_VALUE 700.f // Newtons?

#define RAYCAST_ANGLE 2.5307f           // (~145 degrees) //2.0944f // Radians (~120 degerees from vertical y)
#define GROUND_DISTANCE_THRESHOLD 0.04f // m
// #define MAX_CLIMBABLE_ANGLE 0.7854f // Radians (~45 degrees)

#define JUMP_COOLDOWN_TIME 0.15f // s
#define TURN_TIMEFACTOR .1f      // s

const std::string animation_names[5] = {
    "idle",
    "walk",
    "sprint",
    "in_air",
    "land"};

void PlayerMovementSystem::setPlayerAnimations() {
    if (player_current_state != player_previous_state) {

        AnimationSequenceInfo new_sequence = AnimationSequenceInfo();

        // default animation sequence settings, not used by all blends
        new_sequence.blend_mode = true;

        new_sequence.animation1 = animation_names[player_previous_state];
        new_sequence.anim1_start_from_beginning = false;
        new_sequence.anim1_loop = true;

        new_sequence.animation2 = animation_names[player_current_state];
        new_sequence.anim2_start_from_beginning = true;
        new_sequence.anim2_loop = true;

        // structural stuff for blending - keep for now

        switch (player_previous_state) {
        case PlayerState::Idle:
            switch (player_current_state) {
            case PlayerState::Walking:
                //start walking animation - blend

                new_sequence.blend_duration = .3f;
                break;

            case PlayerState::Running:
                // start running animation
                new_sequence.blend_duration = .3f;
                break;

            case PlayerState::InAir:
                // start jumping animation
                new_sequence.blend_duration = .3f;
                break;
            default:
                break;
            }
            break;
        case PlayerState::Walking:
            switch (player_current_state) {
            case PlayerState::Idle:
                //start idle animation - blend
                new_sequence.blend_duration = .3f;
                break;
            case PlayerState::Running:
                // start running animation
                new_sequence.blend_duration = .2f;
                break;
            case PlayerState::InAir:
                // start jumping animation
                new_sequence.blend_duration = .3f;
                break;
            default:
                break;
            }
            break;
        case PlayerState::Running:
            switch (player_current_state) {
            case PlayerState::Walking:
                //start walking animation - blend
                new_sequence.blend_duration = 1.0f;
                break;
            case PlayerState::Idle:
                // start idle animation
                new_sequence.blend_duration = .2f;
                break;
            case PlayerState::InAir:
                // start jumping animation
                new_sequence.blend_duration = .3f;
                break;
            default:
                break;
            }
            break;
        case PlayerState::InAir:
            new_sequence.blend_mode = true;

            new_sequence.animation1 = animation_names[PlayerState::Landing];
            new_sequence.anim1_start_from_beginning = true;
            new_sequence.anim1_loop = false;

            new_sequence.animation2 = animation_names[player_current_state];
            new_sequence.anim2_start_from_beginning = true;
            new_sequence.anim2_loop = true;

            switch (player_current_state) {
            case PlayerState::Walking:
                //start walking animation - blend
                new_sequence.blend_duration = .3f;
                break;
            case PlayerState::Running:
                // start running animation
                new_sequence.blend_duration = .3f;
                break;
            case PlayerState::Idle:
                // start idle animation
                new_sequence.blend_duration = .3f;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        animation_stack->setAnimationSequence(new_sequence);

        // animation_stack->pauseAnimation(animation_names[player_previous_state]);
        // animation_stack->startAnimation(animation_names[player_current_state]);
    }
}

void PlayerMovementSystem::setPlayerState(bool f, bool b, bool r, bool l, bool space, bool shift) {
    player_previous_state = player_current_state;

    if (on_ground) {
        if (((f ^ b) || (r ^ l)) && !space) {
            if (shift) {
                player_current_state = PlayerState::Running;
            } else
                player_current_state = PlayerState::Walking;
        } else if (space) {
            player_current_state = PlayerState::InAir;
        } else
            player_current_state = PlayerState::Idle;
    } else
        player_current_state = PlayerState::InAir;
}

PlayerMovementSystem::PlayerMovementSystem(PhysicsSystem *physics_in, Player *playerptr, AnimationStack *animation_stack_in) : physics_system(physics_in), player(playerptr), camera(nullptr), animation_stack(animation_stack_in) {
    player_body = player->getPhysicsComponent()->getCollisionObject();
    last_tick_position = player_body->getWorldTransform().getOrigin();
    last_tick_time = 1.f / 60.f;

    // Restrict any player hitbox rotation to only be around the y axis
    player_body->setAngularFactor(btVector3(0.f, 1.f, 0.f));

    // Prevent the player from disabling itself
    player_body->setActivationState(DISABLE_DEACTIVATION);

    // Assume off ground by default
    on_ground = false;
    current_ground = nullptr;
    flying = false;

    jump_cool_down = 0;
    in_air_time = 0;

    player_current_state = PlayerState::Idle;
    player_previous_state = PlayerState::Idle;

    AnimationSequenceInfo new_sequence = AnimationSequenceInfo();

    // default animation sequence settings, not used by all blends
    new_sequence.blend_mode = false;

    new_sequence.animation1 = animation_names[player_previous_state];
    new_sequence.anim1_start_from_beginning = false;
    new_sequence.anim1_loop = true;

    new_sequence.animation2 = animation_names[player_current_state];
    new_sequence.anim2_start_from_beginning = false;
    new_sequence.anim2_loop = true;

    animation_stack->setAnimationSequence(new_sequence);
}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::updatePlayerMovement(bool f, bool b, bool r, bool l, bool space, bool shift, double dt) {
    if (flying) {
        flyPlayer(f, b, r, l, space, shift, dt);
    } else {
        movePlayer(f, b, r, l, space, shift, dt);
    }
}

void PlayerMovementSystem::movePlayer(bool f, bool b, bool r, bool l, bool space, bool shift, double dt) {
    // Ensure player_body has an up to date collision object
    player_body = player->getPhysicsComponent()->getCollisionObject();
    last_tick_position = player_body->getWorldTransform().getOrigin();
    last_tick_time = (float)dt;

    // Calculate control variables for this timestep
    if (!on_ground && in_air_time > 0) in_air_time -= (float)dt;
    testOnGround();
    if (on_ground && jump_cool_down > 0) jump_cool_down -= (float)dt;
    bool moving = f || b || r || l || space || shift;
    btScalar camera_th = btScalar(camera == nullptr ? 0.f : camera->getTh());

    // Get current transform information
    btVector3 current_velocity = player_body->getLinearVelocity();
    btQuaternion current_orientation = player_body->getOrientation();
    btVector3 current_position = player_body->getWorldTransform().getOrigin();

    // Get the angle and axis of the player's current orientation in the same scale as camera_th
    btScalar current_th = current_orientation.getAngle();
    btVector3 current_axis = current_orientation.getAxis();
    if (current_th > 3.1416) {
        current_th -= btScalar(6.2832);
    }
    if (current_axis.getY() < 0) {
        current_th *= -1;
    }

    // Get a vector of the input movements relative to the camera
    btVector3 camera_relative_move_dir = -btVector3(int(f - b) * sin(camera_th) + int(l - r) * cos(camera_th), 0.f, int(f - b) * cos(camera_th) + int(l - r) * -sin(camera_th));

    // Set friction to 0 by default and change as necessary
    player_body->setFriction(0.f);
    player_body->setDamping(0.f, 0.f);
    player_body->setGravity(btVector3(0.f, btScalar(DEFAULT_GRAVITY), 0.f));

    // Only rotate if there is a non-canceled movement input
    if ((f ^ b) || (r ^ l)) {
        btScalar desired_angle = btVector3(0.f, 0.f, 1.f).angle(camera_relative_move_dir.normalize());
        btScalar desired_cross = btVector3(0.f, 0.f, 1.f).cross(camera_relative_move_dir.normalize()).getY();
        btScalar desired_th = desired_cross < 0.f ? -desired_angle : desired_angle;

        // Calculate the angle difference and bound it appropriately
        btScalar delta_th = desired_th - current_th;
        if (delta_th < -3.1416) {
            delta_th += btScalar(6.2832);
        } else if (delta_th > 3.1416) {
            delta_th -= btScalar(6.2832);
        }

        if (abs(delta_th) < .01) {
            // Stop rotating
            player_body->setAngularVelocity(btVector3(0.f, 0.f, 0.f));
        } else {
            // Apply angular velocity
            btVector3 turn_axis(0.f, 1.f, 0.f);
            btScalar turn_value = btScalar(delta_th / TURN_TIMEFACTOR);
            btVector3 angular_velocity = turn_value * turn_axis;
            player_body->setAngularVelocity(angular_velocity);
        }
    }

    // Test movement scenarios
    if ((f ^ b) || (r ^ l)) {
        moved_last_tick = true;

        // Create a normal vector in the direction the player is moving
        btVector3 player_move_dir(sin(current_th), 0.f, cos(current_th));

        if (on_ground) {
            // OLD - use for test
            // btVector3 player_move_along_plane = player_move_dir;

            // NEW - Project the movement vector along the plane the player is currently on
            btVector3 player_move_along_plane = ground_contact_normal.cross(player_move_dir.cross(ground_contact_normal)).normalize();
            if (player_move_along_plane.getY() > .7071) {
                player_move_along_plane.setY(0.f);
                player_body->setLinearFactor(btVector3(1.f, 0.f, 1.f));
            } else {
                player_body->setLinearFactor(btVector3(1.f, 1.f, 1.f));
            }

            // Set the linear velocity of the player body in the desired direction of movement
            btVector3 new_player_velocity = player_move_along_plane * (shift ? GROUND_SPRINT_SPEED : GROUND_MOVE_SPEED);
            new_player_velocity.setY(new_player_velocity.getY() - .1f);
            player_body->setLinearVelocity(new_player_velocity);
            player_body->setDamping(0.f, 0.f);
        } else {
            player_body->setLinearFactor(btVector3(1.f, 1.f, 1.f));

            // Set the linear velocity to the player body in the desired direction of movement
            btVector3 applied_force = player_move_dir * AIR_MOVE_FORCE;
            player_body->applyCentralForce(applied_force);
            player_body->setDamping(.5f, 0.f);
        }
    } else {
        moved_last_tick = false;
        player_body->setLinearFactor(btVector3(1.f, 1.f, 1.f));
        // NOT MOVING
        if (on_ground) {
            // Set friction high to prevent sliding
            player_body->setFriction(10.f);
            player_body->setDamping(.999f, .999f);
            if (moved_last_tick) {
                player_body->applyCentralImpulse(btVector3(0.f, -1000.f, 0.f));
            }
        }
    }

    // Test for jumping conditions
    if (space && on_ground && jump_cool_down <= 0) {
        // Jumping time
        current_velocity.setY(0.f);
        player_body->setLinearVelocity(current_velocity);
        player_body->applyCentralImpulse(btVector3(0.f, JUMP_IMPULSE_VALUE, 0.f));
        on_ground = false;
        jump_cool_down = JUMP_COOLDOWN_TIME;
        in_air_time = .2f;
    }

    setPlayerState(f, b, r, l, space, shift);
    setPlayerAnimations();
}

void PlayerMovementSystem::makePostPhysicsAdjustments() {
    if (!flying) {
        btVector3 current_position = player_body->getWorldTransform().getOrigin();
        btScalar last_speed = ((current_position - last_tick_position) / last_tick_time).length2();

        // If the player was on the ground before the last physics tick, attempt to keep them on the ground
        if (last_speed > .4 && on_ground) {
            btVector3 current_position = player_body->getWorldTransform().getOrigin();
            btVector3 ray_to = current_position - btVector3(0.f, 500.f, 0.f);
            btCollisionWorld::ClosestRayResultCallback callback(current_position, ray_to);
            physics_system->closestRayCast(current_position, ray_to, callback);

            if (callback.hasHit()) {
                btScalar dist = current_position.distance2(callback.m_hitPointWorld);
                btScalar ground_dot = callback.m_hitNormalWorld.getY();
                if (dist < PLAYER_HEIGHT / 2.f + 1.f) {
                    float adjust_value = (.3f * (1.f - ground_dot));
                    float adjusted_y = PLAYER_HEIGHT / 2.f + adjust_value;
                    player_body->getWorldTransform().setOrigin(callback.m_hitPointWorld + btVector3(0.f, adjusted_y, 0.f));
                }
            }
        }
    }
}

void PlayerMovementSystem::flyPlayer(bool f, bool b, bool r, bool l, bool space, bool shift, double dt) {
    btQuaternion current_orientation = player_body->getOrientation();
    btScalar current_th = current_orientation.getAngle();
    btVector3 current_axis = current_orientation.getAxis();
    if (current_th > 3.1416) {
        current_th -= btScalar(6.2832);
    }
    if (current_axis.getY() < 0) {
        current_th *= -1;
    }

    player_body->setGravity(btVector3(0.f, 0.f, 0.f));
    player_body->setFriction(btScalar(0.f));

    if ((f ^ b) || (r ^ l) || (space ^ shift)) {
        player_body->setDamping(.5f, 0.f);
    } else {
        player_body->setDamping(.8f, 0.f);
    }

    btScalar camera_th = btScalar(camera == nullptr ? 0.f : camera->getTh());
    btVector3 camera_relative_move_dir = -btVector3(btScalar(int(f - b) * sin(camera_th) + int(l - r) * cos(camera_th)), btScalar(shift - space), btScalar(int(f - b) * cos(camera_th) + int(l - r) * -sin(camera_th)));
    btVector3 flyingForce = camera_relative_move_dir * btScalar(dt) * AIR_MOVE_FORCE * 150;

    player_body->applyCentralForce(flyingForce);

    if ((f ^ b) || (r ^ l)) {
        camera_relative_move_dir = -btVector3(int(f - b) * sin(camera_th) + int(l - r) * cos(camera_th), 0.f, int(f - b) * cos(camera_th) + int(l - r) * -sin(camera_th));
        btScalar desired_angle = btVector3(0.f, 0.f, 1.f).angle(camera_relative_move_dir.normalize());
        btScalar desired_cross = btVector3(0.f, 0.f, 1.f).cross(camera_relative_move_dir.normalize()).getY();
        btScalar desired_th = desired_cross < 0.f ? -desired_angle : desired_angle;

        // Calculate the angle difference and bound it appropriately
        btScalar delta_th = desired_th - current_th;
        if (delta_th < -3.1416) {
            delta_th += btScalar(6.2832);
        } else if (delta_th > 3.1416) {
            delta_th -= btScalar(6.2832);
        }

        if (abs(delta_th) < .01) {
            // Stop rotating
            player_body->setAngularVelocity(btVector3(0.f, 0.f, 0.f));
        } else {
            // Apply angular velocity
            btVector3 turn_axis(0.f, 1.f, 0.f);
            btScalar turn_value = btScalar(delta_th / TURN_TIMEFACTOR);
            btVector3 angular_velocity = turn_value * turn_axis;
            player_body->setAngularVelocity(angular_velocity);
        }
    }

    player_previous_state = player_current_state;
    player_current_state = PlayerState::InAir;
    setPlayerAnimations();
}

void PlayerMovementSystem::testOnGround() {
    if (in_air_time <= 0.f) {
        // Perform the ray test to detect ground collisions
        glm::vec3 player_pos = glm::vec3(player->getWorldTransform()[3]);
        btVector3 ray_from(player_pos.x, btScalar(player_pos.y + PLAYER_DIAMETER / 2.f), player_pos.z);

        btVector3 ray_to(player_pos.x, player_pos.y - PLAYER_DIAMETER / 2.f, player_pos.z);
        btCollisionWorld::ClosestRayResultCallback callback(ray_from, ray_to);
        physics_system->closestRayCast(ray_from, ray_to, callback);

        // Assume not on ground and test if the player should be
        on_ground = false;
        current_ground = nullptr;
        if (callback.hasHit()) {
            // std::cout << "Callback results:" << std::endl;
            // std::cout << "    Pos:  " << callback.m_hitPointWorld.getX() << ", " << callback.m_hitPointWorld.getY() << ", " << callback.m_hitPointWorld.getZ() << std::endl;
            // std::cout << "    Norm: " << callback.m_hitNormalWorld.getX() << ", " << callback.m_hitNormalWorld.getY() << ", " << callback.m_hitNormalWorld.getZ() << std::endl;

            float dist_to_collision = (float)ray_from.distance(callback.m_hitPointWorld);
            if (dist_to_collision <= GROUND_DISTANCE_THRESHOLD + PLAYER_DIAMETER / 2.f) {
                on_ground = true;
                ground_contact_position = callback.m_hitPointWorld;
                ground_contact_normal = callback.m_hitNormalWorld;
                current_ground = (Obstacle *)callback.m_collisionObject->getUserPointer();
            }
        }
        if (!on_ground) {
            // Cast 8 rays at angle RAYCAST_ANGLE to test if on the ground (approximating a cone test)
            for (int i = 0; i < 8; i++) {
                // Create the new callback object
                float th = i * 6.2830f / 8.f;
                btVector3 ray_offset(sin(RAYCAST_ANGLE) * cos(th), cos(RAYCAST_ANGLE), sin(RAYCAST_ANGLE) * sin(th));
                ray_to = ray_from + (PLAYER_DIAMETER * ray_offset);
                btCollisionWorld::ClosestRayResultCallback angled_callback(ray_from, ray_to);

                // Cast the ray and test for a hit
                physics_system->closestRayCast(ray_from, ray_to, angled_callback);
                if (angled_callback.hasHit()) {
                    // std::cout << "angled_callback results:" << std::endl;
                    // std::cout << "    Pos:  " << angled_callback.m_hitPointWorld.getX() << ", " << angled_callback.m_hitPointWorld.getY() << ", " << angled_callback.m_hitPointWorld.getZ() << std::endl;
                    // std::cout << "    Norm: " << angled_callback.m_hitNormalWorld.getX() << ", " << angled_callback.m_hitNormalWorld.getY() << ", " << angled_callback.m_hitNormalWorld.getZ() << std::endl;

                    float dist_to_collision = (float)ray_from.distance(angled_callback.m_hitPointWorld);
                    if (dist_to_collision <= GROUND_DISTANCE_THRESHOLD + PLAYER_DIAMETER / 2.f) {
                        on_ground = true;
                        ground_contact_position = angled_callback.m_hitPointWorld;
                        ground_contact_normal = angled_callback.m_hitNormalWorld;
                        current_ground = (Obstacle *)angled_callback.m_collisionObject->getUserPointer();
                        break;
                    }
                }
            }
        }

        if (on_ground) {
            // std::cout << "Norm: " << ground_contact_normal.length2() << " " << ground_contact_normal.getX() << ", " << ground_contact_normal.getY() << ", " << ground_contact_normal.getZ() << std::endl;
            // std::cout << "Pos:: " << ground_contact_position.getX() << ", " << ground_contact_position.getY() << ", " << ground_contact_position.getZ() << std::endl;
        }
    }
}
