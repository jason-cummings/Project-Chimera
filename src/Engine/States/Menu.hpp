#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

#include "../GameState.hpp"
#include "../GameObjects/MenuButton.hpp"
#include "../GameObjects/Camera.hpp"

class Menu: public GameState{
protected:
	//Stores vector of buttons and last clicked button
	MenuButton* lastpressed;
	std::vector<MenuButton*> buttons;
	
	// Performs actions of clicked buttons
	virtual void handleButtonEvent( MenuButton* clicked ) {};

public:
	Menu();
	virtual ~Menu();

	// Checks if any button is clicked
	virtual void handleMouseButtonDown( SDL_Event e ) override;
	virtual void handleMouseButtonUp( SDL_Event e ) override;

	virtual void gameLoop() override;
};

#endif