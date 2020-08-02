#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include "GameObject.hpp"
#include "GameObjects/Camera.hpp"
#include "Systems/Rendering/RenderSystem.hpp"

class GameState {
protected:
    // The scene graph for the state
    GameObject *scene;

    // Subsystems for all game states
    RenderSystem & render_system;

    // Is the state already initialized?
    bool is_initialized = false;

    // Variable to determine whether or not this state should lock the mouse
    bool mouse_lock;

    // Variables to be accessed by Engine for transitioning between states
    bool quit_game;
    bool should_destroy_on_state_change;
    GameState *next_state;

    // Set the state to be swapped to
    void setNextState( GameState * input, bool destroy_self );

    // Event handlers to be used in all states
    void handleKeyDown( SDL_Event e );
    void handleKeyUp( SDL_Event e );
    void handleMouseMotion( SDL_Event e );
    void handleMouseButtonDown( SDL_Event e );
    void handleMouseButtonUp( SDL_Event e );
    void handleMouseWheel( SDL_Event e );

    // State-specific event handlers default to doing nothing, so a state can implement only what it needs
    virtual void handleKeyDownStateSpecific( SDL_Event e ) {}
    virtual void handleKeyUpStateSpecific( SDL_Event e ) {}
    virtual void handleMouseMotionStateSpecific( SDL_Event e ) {}
    virtual void handleMouseButtonDownStateSpecific( SDL_Event e ) {}
    virtual void handleMouseButtonUpStateSpecific( SDL_Event e ) {}
    virtual void handleMouseWheelStateSpecific( SDL_Event e ) {}

public:
    GameState();
    virtual ~GameState();

    inline bool isInitialized() { return is_initialized; }
    inline bool shouldDestroy() { return should_destroy_on_state_change; }
    inline bool getQuitGame() { return quit_game; }
    inline bool shouldLockMouse() { return mouse_lock; }
    inline GameObject* getScene() { return scene; }

    /**
     * Perform any necessary state initialization
     * This will be called after any previous state has been completely cleaned up,
     *  guaranteeing no problems with order of operations in render_system
     */
    virtual bool init() = 0;

    /**
     * Tick function called by Engine
     * Responsible for updating all state systems, including rendering
     */
    virtual void gameLoop() = 0;

    /**
     * Call the appropriate event handler for an SDL_Event
     * This can be overwritten if additional event handling is necessary in a subclass
     */
    virtual void handleSDLEvent( SDL_Event e );

    /**
     * Called from engine on a window reshape
     * @param new_width - updated width of window
     * @param new_height - updated height of window
     */
    virtual void reshape( int new_width, int new_height ) {
        render_system.reshape( new_width, new_height );
    }

    // Returns the next state and clear it from as the next state
    GameState* getNextState();
    
    inline void transitionTo() { render_system.populateRenderLists(scene); }
};

#endif