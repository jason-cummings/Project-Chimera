#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Menu.hpp"

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