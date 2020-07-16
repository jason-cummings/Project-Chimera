#ifndef LEVELSELECTMENU_H
#define LEVELSELECTMENU_H

#include <string>
#include <vector>

#include "Menu.hpp"
#include "../Systems/Animation/AnimationSystem.hpp"
#include "../StandardTimer.hpp"

class LevelSelectMenu: public Menu {
private:
    // Systems for main menu
    AnimationSystem * animation_system;
    StandardTimer timer;

    // Camera to be used for 3D objects in the world (not overlay/HUD meshes)
    Camera *camera;

    int selected_level;
    std::vector<MenuElement*> level_images;

    void handleButtonEvent( MenuButton *clicked ) override;

public:
    LevelSelectMenu( GameObject *scene_in, Camera *camera_in, AnimationSystem *anim_in );
    ~LevelSelectMenu();
    
    void gameLoop() override;
};

#endif
