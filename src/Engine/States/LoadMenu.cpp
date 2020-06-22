#include "LoadMenu.hpp"

#include "InGameState.hpp"

#define LOAD_BUTTON_ID "Loading"

LoadMenu::LoadMenu( std::string to_load ) {
    MenuElement *loading_image = new MenuElement(LOAD_BUTTON_ID, .5f, .5f, 1.778f, 1.f, "LoadingScreen");
    scene->addChild( loading_image );
    
    level_name = to_load;
}

LoadMenu::~LoadMenu() {
    scene->setDestroyAll( true );
    delete scene;
}

void LoadMenu::gameLoop() {
    // Boolean to allow loading screen to render once before hanging the gameloop on loading
    if( will_load ) {
        InGameState *to_set = new InGameState( level_name );
        setNextState( to_set, true );
    }
    else {
        will_load = true;
    }
    Menu::gameLoop();
}
