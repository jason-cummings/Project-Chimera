#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Menu.hpp"

class PauseMenu : public Menu {

private:
    void handleButtonEvent( MenuButton* clicked ) override;
    void handleKeyDownStateSpecific( SDL_Event e ) override;
    GameState* last_state;

    void unpause();

public:
    PauseMenu( GameState* input );
    ~PauseMenu();

    inline bool init() override {  is_initialized = true; return true; }
};

#endif