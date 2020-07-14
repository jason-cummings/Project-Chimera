#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

#include "../GameState.hpp"
#include "../GameObjects/MenuButton.hpp"

class Menu: public GameState{
protected:
	//Stores vector of buttons and last clicked button
	MenuButton* lastpressed;
	std::vector<MenuButton*> buttons;
	
	// Convert pixels to menu coordinates
	glm::vec2 getMenuCoords( int screen_x, int screen_y );

	// Performs actions of clicked buttons
	virtual void handleButtonEvent( MenuButton* clicked ) {};

public:
	Menu();
	virtual ~Menu();

	// Checks if any button is clicked
    void handleMouseButtonDownStateSpecific( SDL_Event e ) override;
    void handleMouseButtonUpStateSpecific( SDL_Event e ) override;

	virtual void gameLoop() override;
};

#endif
