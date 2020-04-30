#include "MainMenu.hpp"

MainMenu::MainMenu(){
    scene = new GameObject("root");

    lastpressed = nullptr;
    buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .25f, 1.f, .5f ) );
    buttons.push_back( new MenuButton( EXIT_GAME_BUTTON_ID, .5f, .75f, 1.f, .5f ) );

    camera = new Camera();
    camera->setOffset( 10.f );
}

MainMenu::~MainMenu(){
    for( int i = 0; i < buttons.size(); i++ ) {
        delete buttons[i];
    }
    delete camera;
}

void MainMenu::handleMouseButtonDown( SDL_Event e ) {
    // On Left Click
    if( e.button.button == SDL_BUTTON_LEFT ) {
        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        int w_width = camera->getViewWidth();
        int w_height = camera->getViewHeight();
        float menu_x = x / (float)w_height - (((w_width / (float)w_height) - 1) / 2.f);
        float menu_y = y / (float)w_height;

        // Find button clicked - set to lastpressed
        for(int i = 0; i < buttons.size(); i++){
            if(buttons[i]->clickTest(menu_x,menu_y)){
                lastpressed = buttons[i];
                // std::cout << "Downed " << lastpressed->getID() << std::endl;
            }
        }
    }
}

void MainMenu::handleMouseButtonUp( SDL_Event e ){

    if( e.button.button == SDL_BUTTON_LEFT ) {
        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        int w_width = camera->getViewWidth();
        int w_height = camera->getViewHeight();
        float menu_x = x / (float)w_height - (((w_width / (float)w_height) - 1) / 2.f);
        float menu_y = y / (float)w_height;

        if(lastpressed->clickTest(menu_x,menu_y)) {
            handleButtonEvent(lastpressed);
        }
    }
}


void MainMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        InGameState *to_set = new InGameState("RaptorLand");
        setNextState( to_set );
    }
    else if( clicked->getID() == EXIT_GAME_BUTTON_ID ) {
        quit_game = true;
    }
}
