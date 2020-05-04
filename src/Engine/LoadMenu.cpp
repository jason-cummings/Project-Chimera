#include "LoadMenu.hpp"

LoadMenu::LoadMenu(){
    scene = new GameObject("root");

	lastpressed = nullptr;

	//Insert buttons here like such:
	//buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "MainMenuPlay" ) );
	//
    buttons.push_back( new MenuButton(LOAD_BUTTON_ID, .5f, .5f, 1.778f, 1.f, "LoadingScreen"));

	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}
    camera = new Camera();
}

LoadMenu::~LoadMenu(){
    for( int i = 0; i < buttons.size(); i++ ) {
        delete buttons[i];
    }
}

void LoadMenu::gameLoop(){
    if(loaded){
        InGameState* to_set = new InGameState("Towers");
        setNextState( to_set );
    }
    else{
        loaded = true;
    }
    Menu::gameLoop();
}