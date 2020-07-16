#include "LoadMenu.hpp"

#include "InGameState.hpp"
#include "../Levels/TowersLevel.hpp"
#include "../Levels/CastleLevel.hpp"

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
        Level *to_load = nullptr;
        if( level_name == "Towers" ) {
            to_load = new TowersLevel();
        }
        else if( level_name == "Castle" ) {
            to_load = new CastleLevel();
        }
        to_load->populateLevel();

        InGameState *to_set = new InGameState( to_load );
        setNextState( to_set, true );
    }
    else {
        will_load = true;
    }
    Menu::gameLoop();
}
