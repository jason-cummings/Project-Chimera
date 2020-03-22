#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "GameState.hpp"
#include "GameObjects/Obstacle.hpp"
#include "GameObjects/StaticCubeObstacle.hpp"
#include "./Systems/PlayerMovement/PlayerMovementSystem.hpp"
#include "./GameObjects/Player.hpp"

class InGameState: public GameState {
private:
    bool w, a, s, d, shift, space;
    Player* player;
    PlayerMovementSystem* playerMovement;
public:
    InGameState();
    void update( double dt );
    void handleSDLEvent( SDL_Event e );

    // Perform any necessary updates before the physics step
    virtual void prePhysics();

    // Perform any necessary updates after the physics step
    virtual void postPhysics();
};

#endif