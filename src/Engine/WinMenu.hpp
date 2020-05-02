#ifndef WINMENU_HPP
#define WINMENU_HPP
#include "Menu.hpp"
#include "InGameState.hpp"

//Definitions for buttons
#define CONGRATS_BUTTON_ID "Congrats"
#define MAIN_MENU_BUTTON_ID "Main Menu"




class WinMenu : public Menu {
public:
    WinMenu();
    ~WinMenu();

private:
    void handleButtonEvent( MenuButton* clicked ) override;

};




#endif