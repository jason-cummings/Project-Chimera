#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.hpp"
#include "../Systems/Animation/AnimationSystem.hpp"
#include "../StandardTimer.hpp"

class MainMenu : public Menu {
private:
    // Systems for main menu
    AnimationSystem * animation_system;
    StandardTimer timer;

    // The 3D world in the background
    // Stored to easily pass just the world to the next state
    GameObject *background_scene;

    // Camera to be used for 3D objects in the world (not overlay/HUD meshes)
    Camera *camera;

    void handleButtonEvent( MenuButton *clicked ) override;

public:
    MainMenu();
    ~MainMenu();

    bool init() override;

    void gameLoop() override;
};

#endif
