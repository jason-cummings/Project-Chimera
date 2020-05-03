#ifndef LOADMENU_HPP
#define LOADMENU_HPP
#include "Menu.hpp"
#include "InGameState.hpp"


//Definitions for buttons
#define LOAD_BUTTON_ID "Loading"



class LoadMenu : public Menu {
public:
    LoadMenu();
    ~LoadMenu();

private:
    void gameLoop() override;
    bool loaded = false;

};




#endif