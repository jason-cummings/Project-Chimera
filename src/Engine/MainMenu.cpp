#include "MainMenu.hpp"

MainMenu::MainMenu(){
    scene = new GameObject("root");

    lastpressed = nullptr;
    buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .55f, .6f, .3f, "MainMenuPlay" ) );
    buttons.push_back( new MenuButton( EXIT_GAME_BUTTON_ID, .5f, .2f, .6f, .3f, "MainMenuExit" ) );
    buttons.push_back( new MenuButton( "logo", .5f, .85f, 1.4f, .35f, "MainMenuChimeraLogo" ) );

    // Add to scene graph for rendering
    for( MenuButton *b : buttons ) {
        scene->addChild(b);
    }

    // Create a camera to avoid segfaulting
    camera = new Camera();
    render_system.registerCamera( camera );
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

void MainMenu::handleMouseButtonUp( SDL_Event e ){
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


void MainMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        setNextState( new LoadMenu() );
    }
    else if( clicked->getID() == EXIT_GAME_BUTTON_ID ) {
        quit_game = true;
    }
}

void MainMenu::gameLoop() {
    render_system.render( 0.f, scene );
}
