#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <string>

#include "./../GameObject.hpp"

class MenuButton : public GameObject
{
private:
    float xcoord, ycoord, width, height;
public:
    MenuButton(std::string id, float xinput, float yinput, float winput, float hinput);
    ~MenuButton();

    bool clickTest(float xinput, float yinput);
};







#endif