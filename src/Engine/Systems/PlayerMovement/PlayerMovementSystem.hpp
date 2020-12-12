#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H

#include "./../../GameObjects/Camera.hpp"
#include "./../../GameObjects/Obstacle.hpp"
#include "./../../GameObjects/Player.hpp"
#include "./../Animation/AnimationStack.hpp"
#include "./../Physics/PhysicsSystem.hpp"

enum PlayerState {
    Idle = 0,
    Walking = 1,
    Running = 2,
    InAir = 3,
    Landing = 4
};

class PlayerMovementSystem {
  private:
    Player *player;
    Camera *camera;
    PhysicsSystem *physics_system;
    btRigidBody *player_body;
    AnimationStack *animation_stack;

    bool on_ground, moved_last_tick;
    btVector3 ground_contact_normal, ground_contact_position, last_tick_position;
    Obstacle *current_ground;

    float jump_cool_down, in_air_time, last_tick_time;

    PlayerState player_current_state;
    PlayerState player_previous_state;

    bool flying;

    void setPlayerAnimations();
    void setPlayerState(bool f, bool b, bool r, bool l, bool space, bool shift);

    // On ground movement
    void movePlayer(bool f, bool b, bool r, bool l, bool space, bool shift, double dt);
    // Old movement style
    void flyPlayer(bool f, bool b, bool r, bool l, bool space, bool shift, double dt);

  public:
    PlayerMovementSystem(PhysicsSystem *physics_in, Player *playerptr, AnimationStack *animation_stack_in);
    ~PlayerMovementSystem();

    void updatePlayerMovement(bool f, bool b, bool r, bool l, bool space, bool shift, double dt);

    // Perform any necessary operations after the physics step
    void makePostPhysicsAdjustments();

    // Perform tests to determine whether or not the player is grounded
    void testOnGround();

    inline void registerCamera(Camera *input) { camera = input; }
    inline void toggleFlying() { flying = !flying; }
};

#endif