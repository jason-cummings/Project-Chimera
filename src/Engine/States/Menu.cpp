#include "Menu.hpp"
Menu::Menu(){
	scene = new GameObject("root");

    mouse_lock = false;
	lastpressed = nullptr;

	// Insert buttons here like such:
	// buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "MainMenuPlay" ) );
	//

	for( MenuButton* b : buttons ){
		scene->addChild(b);
	}
}

Menu::~Menu(){
	for( int i = 0; i < buttons.size(); i++ ) {
        delete buttons[i];
    }
}

void Menu::handleMouseButtonDown( SDL_Event e ) {
    // On Left Click
    if( e.button.button == SDL_BUTTON_LEFT ) {
        lastpressed = nullptr;

        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        int w_width = camera->getViewWidth();
        int w_height = camera->getViewHeight();
        y = w_height - y;

        float menu_x = x / (float)w_height - (((w_width / (float)w_height) - 1) / 2.f);
        float menu_y = y / (float)w_height;

        // std::cout << "Clicked at " << menu_x << ", " << menu_y << std::endl;

        // Find button clicked - set to lastpressed
        for( int i = 0; i < buttons.size(); i++ ) {
            if( buttons[i]->clickTest(menu_x,menu_y) ) {
                lastpressed = buttons[i];
                // std::cout << "Downed " << lastpressed->getID() << std::endl;
            }
        }
    }
}

void Menu::handleMouseButtonUp( SDL_Event e ){
    if( e.button.button == SDL_BUTTON_LEFT ) {
        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        int w_width = camera->getViewWidth();
        int w_height = camera->getViewHeight();
        y = w_height - y;

        float menu_x = x / (float)w_height - (((w_width / (float)w_height) - 1) / 2.f);
        float menu_y = y / (float)w_height;

        if( lastpressed != nullptr && lastpressed->clickTest(menu_x,menu_y) ) {
            handleButtonEvent(lastpressed);
        }
    }
}


void Menu::handleButtonEvent( MenuButton *clicked ) {
    //Series of if statements that handle what happens after a button gets clicked.
	// In the format of :

	/*if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        InGameState *to_set = new InGameState("Diving");
        setNextState( to_set );
    }
    else if( clicked->getID() == EXIT_GAME_BUTTON_ID ) {
        quit_game = true;
    }*/

}

void Menu::gameLoop() {
    if( next_state == nullptr ) {
        render_system.render( 0.f, scene );
    }
}


