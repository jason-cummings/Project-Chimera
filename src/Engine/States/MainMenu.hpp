#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.hpp"
#include "../GameObjects/MenuButton.hpp"
#include "../Systems/Animation/AnimationSystem.hpp"
#include "../StandardTimer.hpp"

class MainMenu : public Menu {
private:
    // Systems for main menu
    AnimationSystem * animation_system;
    StandardTimer timer;

    void handleButtonEvent( MenuButton *clicked );

    void init() override {}

public:
    MainMenu();
    ~MainMenu();

    void handleMouseButtonDown( SDL_Event e ) override;
    void handleMouseButtonUp( SDL_Event e ) override;

    void gameLoop() override;
};

#endif