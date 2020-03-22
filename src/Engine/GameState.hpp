#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include "GameObject.hpp"

class GameState {
protected:
    GameObject *scene;

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

    virtual void handleSDLEvent( SDL_Event e ) = 0;

    GameObject *getScene() { return scene; }
};

#endif