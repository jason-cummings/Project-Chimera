#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "./../../GameObjects/Player.hpp"
#include "./../../GameObjects/Camera.hpp"
#include "./../Physics/PhysicsSystem.hpp"

#define MOVE_SPEED 5.f // m/s
#define GROUND_DISTANCE_THRESHOLD 0.1f // m
#define JUMP_COOLDOWN_TIME 0.15f // s
#define JUMP_IMPULSE_VALUE 500.f // Newtons?

class PlayerMovementSystem
{
private:
    Player *player;
    Camera *camera;
    PhysicsSystem *physics_system;
    btRigidBody *player_body;

    bool on_ground;
    float jump_cool_down;

public:
    PlayerMovementSystem( PhysicsSystem *physics_in, Player* playerptr );
    ~PlayerMovementSystem();

    // On ground movement
    void movePlayer( bool f, bool b, bool r, bool l, bool space, bool shift, double dt );

    // Old movement style
    void flyPlayer( int ad, int ss, int ws, double dt );

    // Perform tests to determine whether or not the player is grounded
    void testOnGround();

    inline void registerCamera( Camera* input ) { camera = input; }
};






#endif