#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <string>
#include <GL/glew.h>

#include "MenuElement.hpp"

class MenuButton: public MenuElement
{
private:
public:
    MenuButton(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    ~MenuButton();

    bool clickTest( float xinput, float yinput );
};


#endif