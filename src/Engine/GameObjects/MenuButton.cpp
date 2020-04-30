#include "MenuButton.hpp"

MenuButton::MenuButton(std::string id, float xinput, float yinput, float winput, float hinput ): GameObject(id) {
    xcoord = xinput;
    ycoord = yinput;
    width = winput;
    height = hinput;
}

MenuButton::~MenuButton(){
    
}

bool MenuButton::clickTest(float xinput, float yinput){
    return (yinput > ycoord - height / 2.f && yinput < ycoord + height / 2.f && xinput > xcoord - width / 2.f && xinput < xcoord + width / 2.f); 
}