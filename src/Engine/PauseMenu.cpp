#include "PauseMenu.hpp"

PauseMenu::PauseMenu( GameState* input ){
    last_state = input;
    scene = new GameObject("root");
    scene->addChild( input->getScene() );

	lastpressed = nullptr;

	//Insert buttons here like such:
	//buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "MainMenuPlay" ) );

	//One button to resume game, one to return to main menu.
    buttons.push_back( new MenuButton(RESUME_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "PauseMenuResume"));
    buttons.push_back( new MenuButton(MAIN_MENU_BUTTON_ID, .5f, .75f, .6f, .3f, "PauseMenuMainMenu"));


	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}

	// Create a camera to avoid segfaulting
    camera = new Camera();
}

PauseMenu::~PauseMenu(){
	for( int i = 0; i < buttons.size(); i++ ) {
        delete buttons[i];
    }
    delete camera;
}

void PauseMenu::handleButtonEvent( MenuButton *clicked ) {
    //Series of if statements that handle what happens after a button gets clicked.
	// In the format of :

	/*if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        InGameState *to_set = new InGameState("Diving");
        setNextState( to_set );
    }
    else if( clicked->getID() == EXIT_GAME_BUTTON_ID ) {
        quit_game = true;
    }*/

    if(clicked->getID() == RESUME_GAME_BUTTON_ID){
        //Change state back into game
        scene->removeChild( last_state->getScene() );
        setNextState( last_state );
    }
    else if(clicked->getID() == MAIN_MENU_BUTTON_ID){
        //Change state to Main Menu
        MainMenu* to_set = new MainMenu();
        setNextState( to_set );
    }

}
