#ifndef MENUSLIDER_H
#define MENUSLIDER_H

#include <GL/glew.h>
#include <string>

#include "MenuElement.hpp"

class MenuSlider : public MenuElement {
  private:
  public:
    MenuSlider(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    ~MenuSlider();
};

#endif