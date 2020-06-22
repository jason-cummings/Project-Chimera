#include "LevelSelectMenu.hpp"

#include "MainMenu.hpp"
#include "LoadMenu.hpp"

#define START_GAME_BUTTON_ID "Start"
#define BACK_BUTTON_ID "Back"
#define NEXT_LEVEL_BUTTON_ID "Next"
#define LAST_LEVEL_BUTTON_ID "Last"

const std::string level_options[] = {
    "Towers",
    "Castle"
};

const int n_levels = sizeof(level_options) / sizeof(std::string);

LevelSelectMenu::LevelSelectMenu( GameObject *scene_in, Camera *camera_in, AnimationSystem *anim_in ) {
    selected_level = 0;

    // Set the scene to what was passed in (camera is already childed to scene by MainMenu)
    scene = scene_in;
    camera = camera_in;
    animation_system = anim_in;
    render_system.registerCamera( camera );

    // Buttons to start or go back
    buttons.push_back( new MenuButton( START_GAME_BUTTON_ID, .5f, .35f, .6f, .15f, "LevelSelectMenuStart" ) );
    buttons.push_back( new MenuButton( BACK_BUTTON_ID, .5f, .15f, .6f, .15f, "LevelSelectMenuBack" ) );
    
    // Buttons to choose a level
    MenuButton *next_button = new MenuButton( NEXT_LEVEL_BUTTON_ID, .85f, .7f, .1f, .5f, "LevelSelectMenuNext" );
    next_button->setZLevel(0);
    MenuButton *last_button = new MenuButton( LAST_LEVEL_BUTTON_ID, .15f, .7f, .1f, .5f, "LevelSelectMenuLast" );
    last_button->setZLevel(0);
    buttons.push_back( next_button );
    buttons.push_back( last_button );

    // Border for images
    MenuElement *border = new MenuElement( "Border", .5f, .7f, .8f, .5f, "LevelSelectThumbnailOverlay" );
    border->setZLevel(1);
    scene->addChild( border );

    // Create elements for each 
    for( int i=0; i<n_levels; i++ ) {
        level_images.push_back( new MenuElement("LevelImage", .5f, .7f, .8f, .5f, level_options[i] + "LevelImage" ) );
    }
    for( MenuElement *e : level_images ) {
        e->setZLevel(2);
    }
    
    // Add this the first time to be able to populate the render lists properly
    // Ends up acting as a sort of place holder
    scene->addChild( level_images[0] );
    
	for( MenuButton *b : buttons ) {
		scene->addChild(b);
	}

}

LevelSelectMenu::~LevelSelectMenu() {
    if( render_system.getRegisteredCamera() == camera ) render_system.registerCamera( nullptr );
    scene->removeChild( level_images[0] );
    scene->setDestroyAll( true );
    delete scene;
    for( MenuElement *e : level_images ) {
        delete e;
    }
}

void LevelSelectMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == START_GAME_BUTTON_ID ) {
        // Create the game
        setNextState( new LoadMenu( level_options[selected_level] ), true );
    }
    else if( clicked->getID() == BACK_BUTTON_ID ) {
        // Navigate back to the main menu
        setNextState( new MainMenu(), true );
    }
    else if( clicked->getID() == NEXT_LEVEL_BUTTON_ID ) {
        // Next level
        render_system.removeGameObjectFromRenderLists( level_images[selected_level] );
        selected_level++;
        selected_level %= n_levels;
        render_system.addToRenderLists( level_images[selected_level] );
    }    
    else if( clicked->getID() == LAST_LEVEL_BUTTON_ID ) {
        // Last level
        render_system.removeGameObjectFromRenderLists( level_images[selected_level] );
        selected_level--;
        if( selected_level < 0 ) selected_level += n_levels;
        render_system.addToRenderLists( level_images[selected_level] );
    }
}

void LevelSelectMenu::gameLoop() {
    float dt = (float)timer.getLastTickTime();

    camera->modifyAngles( dt/4.f, 0.f );
    camera->createMatrices();

    animation_system->evaluateAnimations(dt);

    render_system.render( 0.f );
}
