#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include "GameObject.hpp"
#include "GameObjects/Camera.hpp"
#include "LevelLoader.hpp"
#include "StandardTimer.hpp"
#include "Systems/Rendering/RenderSystem.hpp"

class GameState {
protected:
    // The scene graph for the state
    GameObject *scene;

    // Subsystems for all game states
    RenderSystem & render_system;

    // Variable to determine whether or not this state should lock the mouse
    bool mouse_lock = false;

    // Control variable to ensure the state was successfully initialized
    bool init_success;

    // Initialize function to be called in the constructor Should set init_success to false on any failure
    virtual void init() = 0;

    // Camera to be used for 3D objects in the world (not overlay/HUD meshes)
    Camera *camera = nullptr;

    // Variables to be accessed by Engine for transitioning between states
    bool quit_game = false, should_destroy_on_state_change = false;
    GameState *next_state;

    /** 
     * Set the state engine should switch to next
     * @param input - state that Engine should swap to
     * @param destroy_self - should Engine destroy this state after swapping to input
     */
    inline void setNextState( GameState * input, bool destroy_self ) { next_state = input; should_destroy_on_state_change = destroy_self; }

    // Event handlers default to doing nothing, so a state can implement only what it needs
    virtual void handleKeyDown( SDL_Event e ) {}
    virtual void handleKeyUp( SDL_Event e ) {}
    virtual void handleMouseMotion( SDL_Event e ) {}
    virtual void handleMouseButtonDown( SDL_Event e ) {}
    virtual void handleMouseButtonUp( SDL_Event e ) {}
    virtual void handleMouseWheel( SDL_Event e ) {}


public:
    virtual ~GameState() {}
    GameState(): render_system( RenderSystem::getRenderSystem() ) { scene = nullptr; init_success = true; next_state = nullptr; }

    inline bool getInitSuccess() { return init_success; }

    // Tick function called by Engine
    // Responsible for updating all state systems, including rendering
    virtual void gameLoop() = 0;

    // Call the appropriate event handler for an SDL_Event
    // This can be overwritten if additional event handling is necessary in a subclass
    virtual void handleSDLEvent( SDL_Event e ) {
        switch( e.type ) {
            case SDL_KEYDOWN:           handleKeyDown( e );         break;
            case SDL_KEYUP:             handleKeyUp( e );           break;
            case SDL_MOUSEMOTION:       handleMouseMotion( e );     break;
            case SDL_MOUSEBUTTONDOWN:   handleMouseButtonDown( e ); break;
            case SDL_MOUSEBUTTONUP:     handleMouseButtonUp( e );   break;
            case SDL_MOUSEWHEEL:        handleMouseWheel( e );      break;
        }
    }

    /**
     * Called from engine on a window reshape
     * @param new_width - updated width of window
     * @param new_height - updated height of window
     */
    virtual void reshape( int new_width, int new_height ) {
        camera->reshape( new_width, new_height );
        render_system.reshape( new_width, new_height );
    }

    // Returns the next state 
    inline GameState* getNextState() {
        GameState *ret = next_state;
        next_state = nullptr;
        return ret;
    }

    inline bool shouldDestroy() { return should_destroy_on_state_change; }

    inline bool getQuitGame() { return quit_game; }

    // Tell the engine whether or not this state should lock the mouse
    bool shouldLockMouse() { return mouse_lock; }

    GameObject* getScene() { return scene; }
};

#endif