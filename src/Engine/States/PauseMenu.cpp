#include "PauseMenu.hpp"

#include "InGameState.hpp"
#include "MainMenu.hpp"

#define RESUME_GAME_BUTTON_ID "Resume Game"
#define MAIN_MENU_BUTTON_ID "Main Menu"

PauseMenu::PauseMenu( GameState* input ){
    last_state = input;
    scene->addChild( input->getScene() );

	//One button to resume game, one to return to main menu.
    buttons.push_back( new MenuButton( RESUME_GAME_BUTTON_ID, .5f, .58f, .6f, .15f, "PauseMenuResume" ) );
    buttons.push_back( new MenuButton( MAIN_MENU_BUTTON_ID, .5f, .42f, .6f, .15f, "PauseMenuMainMenu" ) );


	for( MenuButton* b : buttons ) {
		scene->addChild(b);
	}

	// Create a camera to avoid segfaulting
    camera = new Camera();
}

PauseMenu::~PauseMenu() {
    // delete camera;
    scene->setDestroyAll( true );
    delete scene;
}

void PauseMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == RESUME_GAME_BUTTON_ID ) {
        // Change state back into game
        scene->removeChild( last_state->getScene() );
        setNextState( last_state, false );
    }
    else if( clicked->getID() == MAIN_MENU_BUTTON_ID ) {
        // Change state to Main Menu 
        scene->removeChild( last_state->getScene() );
        delete last_state;
        MainMenu* to_set = new MainMenu();
        setNextState( to_set, true );
    }
}
