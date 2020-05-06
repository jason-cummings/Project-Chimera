#ifndef WINMENU_HPP
#define WINMENU_HPP

#include "Menu.hpp"

class InGameState;

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