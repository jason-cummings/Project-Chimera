#ifndef MAINMENU_H
#define MAINMENU_H
#include <vector>
#include "GameState.hpp"
#include "InGameState.hpp"
#include "./GameObjects/MenuButton.hpp"
#include "Menu.hpp"
#include "LoadMenu.hpp"

#define PLAY_GAME_BUTTON_ID "Play Game"
#define EXIT_GAME_BUTTON_ID "Exit"

class MainMenu : public Menu
{
private:
    MenuButton * lastpressed;
    std::vector<MenuButton*> buttons;

    AnimationSystem * animation_system;

    void handleButtonEvent( MenuButton *clicked );

    void init() override {}

public:
    MainMenu();
    ~MainMenu();

    void handleMouseButtonDown( SDL_Event e ) override;
    void handleMouseButtonUp( SDL_Event e ) override;

    void gameLoop() override;
    
    bool shouldLockMouse() override { return false; }
};

#endif