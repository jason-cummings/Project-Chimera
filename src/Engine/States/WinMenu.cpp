#include "WinMenu.hpp"

#include "InGameState.hpp"
#include "MainMenu.hpp"

#define CONGRATS_BUTTON_ID "Congrats"
#define MAIN_MENU_BUTTON_ID "Main Menu"

WinMenu::WinMenu(InGameState* input){
    last_state = input;

    scene->addChild( input->getScene() );

    buttons.push_back( new MenuButton( CONGRATS_BUTTON_ID,  .5f, .6f, 1.f, .25f, "CongratsScreen" ) );
    buttons.push_back( new MenuButton( MAIN_MENU_BUTTON_ID, .5f, .4f, .6f, .15f, "PauseMenuMainMenu" ) );

	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}
    
	// Create a camera to avoid segfaulting
    camera = new Camera();
}

WinMenu::~WinMenu() {
    delete last_state;
    scene->setDestroyAll( true );
    delete scene;
}

void WinMenu::handleButtonEvent( MenuButton *clicked ) {
    if(clicked->getID() == MAIN_MENU_BUTTON_ID){
        scene->removeChild(last_state->getScene());
        setNextState( new MainMenu(), true );
    }
}