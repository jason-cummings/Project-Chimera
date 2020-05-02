#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "GameState.hpp"
#include "GameObject.hpp"
#include "GameObjects/Obstacle.hpp"
#include "GameObjects/DynamicCube.hpp"
#include "GameObjects/StaticCubeObstacle.hpp"
#include "./Systems/PlayerMovement/PlayerMovementSystem.hpp"
#include "./Systems/Animation/AnimationSystem.hpp"
#include "Systems/Physics/PhysicsSystem.hpp"
#include "./GameObjects/Player.hpp"

class InGameState;
#include "MainMenu.hpp"
#include "PauseMenu.hpp"
#include "WinMenu.hpp"


class InGameState: public GameState {
private:
    bool w, a, s, d, shift, space;
    Player* player;
    std::string current_level;
    glm::vec3 end_coords;

    // Subsystems for this state
    PlayerMovementSystem* player_movement;
    PhysicsSystem *physics_system;
    AnimationSystem * animation_system;

    // Initialize the state
    void init() override;

    // These override default methods in GameState and do not have to be implemented if necessary
    void handleKeyDown( SDL_Event e ) override;
    void handleKeyUp( SDL_Event e ) override;
    void handleMouseMotion( SDL_Event e ) override;
    // void handleMouseButtonDown( SDL_Event e ) override;
    // void handleMouseButtonUp( SDL_Event e ) override;
    // void handleMouseWheel( SDL_Event e ) override;

    // Perform any necessary updates before the physics step
    void prePhysics();

    // Perform any necessary updates after the physics step
    void postPhysics();

    //Check if player is in endgame location 
    bool endGame();
    bool isNear(float input, float goal, float threshold);
    bool fell();


    // TEMP
    void createTestPhysicsLevel();
    void addPhysicsThings();

public:
    InGameState( std::string level_to_load );
    void gameLoop() override;
};

#endif