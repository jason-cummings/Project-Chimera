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

    // Camera to be used for 3D objects in the world (not overlay/HUD meshes)
    Camera *camera;

    void handleButtonEvent( MenuButton *clicked );

    void init() override {}

public:
    MainMenu();
    ~MainMenu();

    void gameLoop() override;
};

#endif