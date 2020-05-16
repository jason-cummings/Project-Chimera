#ifndef MENUSLIDER_H
#define MENUSLIDER_H

#include <string>
#include <GL/glew.h>

#include "MenuElement.hpp"

class MenuSlider: public MenuElement {
private:

public:
    MenuSlider( std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name );
    ~MenuSlider();
};

#endif