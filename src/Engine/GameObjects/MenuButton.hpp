#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <GL/glew.h>
#include <string>

#include "MenuElement.hpp"

class MenuButton : public MenuElement {
  private:
  public:
    MenuButton(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    ~MenuButton();

    bool clickTest(float xinput, float yinput);
};

#endif