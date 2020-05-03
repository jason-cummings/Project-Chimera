#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP
#include "Menu.hpp"
#include "InGameState.hpp"

//Definitions for buttons
#define RESUME_GAME_BUTTON_ID "Resume Game"
#define MAIN_MENU_BUTTON_ID "Main Menu"




class PauseMenu : public Menu {
public:
    PauseMenu( GameState* input );
    ~PauseMenu();

private:
    void handleButtonEvent( MenuButton* clicked ) override;
    GameState* last_state;

};




#endif