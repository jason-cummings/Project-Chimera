#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include "GameObject.hpp"
#include "./GameObjects/Camera.hpp"
#include "LevelLoader.hpp"
#include "StandardTimer.hpp"
#include "Systems/Rendering/RenderSystem.hpp"

class GameState {
protected:
    // The scene graph for the state
    GameObject *scene;

    // Subsystems for all game states
    RenderSystem & render_system;
    Timer *timer;

    // Control variable to ensure the state was successfully initialized
    bool init_success;

    // Initialize function to be called in the constructor Should set init_success to false on any failure
    virtual void init() = 0;

    // Event handlers default to doing nothing, so a state can implement only what it needs
    virtual void handleKeyDown( SDL_Event e ) {}
    virtual void handleKeyUp( SDL_Event e ) {}
    virtual void handleMouseMotion( SDL_Event e ) {}
    virtual void handleMouseButtonDown( SDL_Event e ) {}
    virtual void handleMouseButtonUp( SDL_Event e ) {}
    virtual void handleMouseWheel( SDL_Event e ) {}
    Camera *camera;

public:
    virtual ~GameState() {}
    GameState(): render_system( RenderSystem::getRenderSystem() ) { scene = nullptr; init_success = true; }

    inline bool getInitSuccess() { return init_success; }

    // Perform any relevant state updates
    virtual void gameLoop() = 0;

    // Update the scene graph to reflect changes
    // This should be done after all systems have executed, but before rendering
    // void updateScene() { scene->update(); }

    // Call the appropriate event handler for an SDL_Event
    // This can be overwritten if additional event handling is necessary in a subclass
    virtual void handleSDLEvent( SDL_Event e )  {
        switch( e.type ) {
            case SDL_KEYDOWN:           handleKeyDown( e );         break;
            case SDL_KEYUP:             handleKeyUp( e );           break;
            case SDL_MOUSEMOTION:       handleMouseMotion( e );     break;
            case SDL_MOUSEBUTTONDOWN:   handleMouseButtonDown( e ); break;
            case SDL_MOUSEBUTTONUP:     handleMouseButtonUp( e );   break;
            case SDL_MOUSEWHEEL:        handleMouseWheel( e );      break;
        }
    }

    virtual void reshape( int new_width, int new_height ) {
        render_system.reshape( new_width, new_height );
    }
};

#endif