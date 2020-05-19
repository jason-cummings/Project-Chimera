#include "MenuButton.hpp"

MenuButton::MenuButton(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name ):
    MenuElement( id, xinput, yinput, winput, hinput, material_name ) {
}

MenuButton::~MenuButton() {}

bool MenuButton::clickTest(float xinput, float yinput){
    return (yinput >= ycoord - height / 2.f && yinput <= ycoord + height / 2.f && xinput >= xcoord - width / 2.f && xinput <= xcoord + width / 2.f); 
}