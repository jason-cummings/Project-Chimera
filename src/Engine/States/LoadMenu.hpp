#ifndef LOADMENU_HPP
#define LOADMENU_HPP

#include "Menu.hpp"
#include "InGameState.hpp"

#define LOAD_BUTTON_ID "Loading"

class LoadMenu: public Menu {
private:
    bool will_load = false;

	virtual void init() override {}

    void gameLoop() override;

public:
    LoadMenu();
    ~LoadMenu();
};

#endif