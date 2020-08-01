#ifndef LOADMENU_HPP
#define LOADMENU_HPP

#include <string>

#include "Menu.hpp"

class LoadMenu: public Menu {
private:
    bool will_load = false;
    std::string level_name;

public:
    LoadMenu( std::string to_load );
    ~LoadMenu();
    
    inline bool init() override { return true; }

    void gameLoop() override;
};

#endif