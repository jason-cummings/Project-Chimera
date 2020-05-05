#ifndef WINMENU_HPP
#define WINMENU_HPP

#include "Menu.hpp"
#include "InGameState.hpp"

#define CONGRATS_BUTTON_ID "Congrats"
#define MAIN_MENU_BUTTON_ID "Main Menu"

class WinMenu : public Menu {
private:
    void handleButtonEvent( MenuButton* clicked ) override;
    GameState* last_state;

	virtual void init() override {}

public:
    WinMenu( InGameState* input );
    ~WinMenu();
};

#endif