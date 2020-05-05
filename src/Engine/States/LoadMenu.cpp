#include "LoadMenu.hpp"

LoadMenu::LoadMenu(){
    scene = new GameObject("root");

	// Insert buttons here like such:
    buttons.push_back( new MenuButton(LOAD_BUTTON_ID, .5f, .5f, 1.778f, 1.f, "LoadingScreen"));

	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}
    camera = new Camera();
}

LoadMenu::~LoadMenu() {}

void LoadMenu::gameLoop(){
    if( will_load ){
        InGameState *to_set = new InGameState("Towers");
        setNextState( to_set, true );
    }
    else {
        will_load = true;
    }
    Menu::gameLoop();
}