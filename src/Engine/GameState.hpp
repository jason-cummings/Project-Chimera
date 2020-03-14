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
    virtual void update( double dt ) = 0;
    virtual void handleSDLEvent( SDL_Event e ) = 0;

    GameObject *getScene() { return scene; }
};

#endif