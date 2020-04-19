#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#include "./../../GameObjects/Player.hpp"
#include "./../../GameObjects/Obstacle.hpp"
#include "./../../GameObjects/Camera.hpp"
#include "./../Physics/PhysicsSystem.hpp"

#define GROUND_MOVE_SPEED 7.f // m/s
#define GROUND_SPRINT_SPEED 12.f // m/s
#define AIR_MOVE_FORCE 1000.f // ?
#define JUMP_IMPULSE_VALUE 500.f // Newtons?

#define RAYCAST_ANGLE 2.5307f // (~145 degrees) //2.0944f // Radians (~120 degerees from vertical y)
#define GROUND_DISTANCE_THRESHOLD 0.04f // m
// #define MAX_CLIMBABLE_ANGLE 0.7854f // Radians (~45 degrees)

#define JUMP_COOLDOWN_TIME 0.15f // s
#define TURN_TIMEFACTOR .1f // s

class PlayerMovementSystem
{
private:
    Player *player;
    Camera *camera;
    PhysicsSystem *physics_system;
    btRigidBody *player_body;

    bool on_ground;
    btVector3 ground_contact_normal, ground_contact_position;
    Obstacle *current_ground;
    
    float jump_cool_down;

public:
    PlayerMovementSystem( PhysicsSystem *physics_in, Player* playerptr );
    ~PlayerMovementSystem();

    // On ground movement
    void movePlayer( bool f, bool b, bool r, bool l, bool space, bool shift, double dt );

    // Perform any necessary operations after the physics step
    void makePostPhysicsAdjustments();

    // Old movement style
    void flyPlayer( int ad, int ss, int ws, double dt );

    // Perform tests to determine whether or not the player is grounded
    void testOnGround();

    inline void registerCamera( Camera* input ) { camera = input; }
};

#endif