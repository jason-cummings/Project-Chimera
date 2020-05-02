#include "WinMenu.hpp"

WinMenu::WinMenu(){
    scene = new GameObject("root");

	lastpressed = nullptr;

	//Insert buttons here like such:
	//buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "MainMenuPlay" ) );
	//

    buttons.push_back( new MenuButton( CONGRATS_BUTTON_ID, .5f, .55f, .6f, .3f, "CongratsScreen"));
    buttons.push_back( new MenuButton(MAIN_MENU_BUTTON_ID, .5f, .35f, .6f, .3f, "PauseMenuMainMenu"));

	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}
    
	// Create a camera to avoid segfaulting
    camera = new Camera();
}

WinMenu::~WinMenu(){
    for( int i = 0; i < buttons.size(); i++ ) {
        delete buttons[i];
    }
}


void WinMenu::handleButtonEvent( MenuButton *clicked ) {
    //Series of if statements that handle what happens after a button gets clicked.
	// In the format of :

    if(clicked->getID() == MAIN_MENU_BUTTON_ID){
        setNextState( new MainMenu() );
    }


}