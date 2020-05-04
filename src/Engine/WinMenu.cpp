#include "WinMenu.hpp"

WinMenu::WinMenu(InGameState* input){
    last_state = input;
    scene = new GameObject("root");
	lastpressed = nullptr;
    scene->addChild( input->getScene() );

	//Insert buttons here like such:

    buttons.push_back( new MenuButton( CONGRATS_BUTTON_ID, .5f, .6f, 1.f, .25f, "CongratsScreen"));
    buttons.push_back( new MenuButton(MAIN_MENU_BUTTON_ID, .5f, .4f, .6f, .15f, "PauseMenuMainMenu"));

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
        scene->removeChild(last_state->getScene());
        setNextState( new MainMenu() );
    }
}