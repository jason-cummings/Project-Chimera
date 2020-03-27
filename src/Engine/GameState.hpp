#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include "GameObject.hpp"
#include "LevelLoader.hpp"

class GameState {
protected:
    GameObject *scene;

    // Event handlers default to doing nothing, so a state can implement only what it needs
    virtual void handleKeyDown( SDL_Event e ) {}
    virtual void handleKeyUp( SDL_Event e ) {}
    virtual void handleMouseMotion( SDL_Event e ) {}
    virtual void handleMouseButtonDown( SDL_Event e ) {}
    virtual void handleMouseButtonUp( SDL_Event e ) {}
    virtual void handleMouseWheel( SDL_Event e ) {}

public:
    virtual ~GameState() {}
    GameState() { scene = nullptr; }

    // Perform any relevant state updates
    virtual void update( double dt ) = 0;

    // Perform any necessary updates before the physics step
    virtual void prePhysics() {};

    // Perform any necessary updates after the physics step
    virtual void postPhysics() {};

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

    GameObject *getScene() { return scene; }
};

#endif