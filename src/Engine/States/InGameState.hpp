#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "../GameState.hpp"
#include "../Levels/Level.hpp"

#include "../Systems/PlayerMovement/PlayerMovementSystem.hpp"
#include "../Systems/Animation/AnimationSystem.hpp"
#include "../Systems/Physics/PhysicsSystem.hpp"

#include "../GameObject.hpp"

#include "../Utilities/PerformanceLogger.hpp"
#include "../StandardTimer.hpp"

class InGameState: public GameState {
private:
    bool w, a, s, d, shift, space;
    Player* player;
    glm::vec3 end_coords;

    // Camera to be used for 3D objects in the world (not overlay/HUD meshes)
    Camera *camera;
    bool player_visible;

    //Stops movement for pause menu
    void movementFalse();
    bool first_tick;

    // Subsystems for this state
    PlayerMovementSystem* player_movement;
    PhysicsSystem *physics_system;
    AnimationSystem * animation_system;
    
    StandardTimer timer;
    PerformanceLogger performance_logger;

    Level *level_used;

    // These override default methods in GameState and do not have to be implemented if necessary
    void handleKeyDownStateSpecific( SDL_Event e ) override;
    void handleKeyUpStateSpecific( SDL_Event e ) override;
    void handleMouseMotionStateSpecific( SDL_Event e ) override;
    // void handleMouseButtonDownStateSpecific( SDL_Event e ) override;
    // void handleMouseButtonUpStateSpecific( SDL_Event e ) override;
    // void handleMouseWheelStateSpecific( SDL_Event e ) override;

    // Perform any necessary updates before the physics step
    void prePhysics();

    // Perform any necessary updates after the physics step
    void postPhysics();

    // Check if player is in endgame location 
    bool endGame();
    bool isNear(float input, float goal);
    bool fell();

    void togglePlayerVisibility();

public:
    InGameState( Level *level );
    ~InGameState();

    bool init() override;

    void gameLoop() override;
};

#endif