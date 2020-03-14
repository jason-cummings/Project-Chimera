#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "GameState.hpp"
#include "GameObjects/Obstacle.hpp"

class InGameState: public GameState {
private:
    double TEMP_ph, TEMP_th;
    bool w, a, s, d;
public:
    InGameState();
    void update( double dt );
    void handleSDLEvent( SDL_Event e );
};

#endif