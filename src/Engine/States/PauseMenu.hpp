#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Menu.hpp"
#include "InGameState.hpp"

#define RESUME_GAME_BUTTON_ID "Resume Game"
#define MAIN_MENU_BUTTON_ID "Main Menu"

class PauseMenu : public Menu {

private:
    void handleButtonEvent( MenuButton* clicked ) override;
    GameState* last_state;

	virtual void init() override {}

public:
    PauseMenu( GameState* input );
    ~PauseMenu();
};

#endif