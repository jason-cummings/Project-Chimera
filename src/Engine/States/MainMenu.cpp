#include "MainMenu.hpp"

#include "LoadMenu.hpp"
#include "../LevelLoader.hpp"

#define PLAY_GAME_BUTTON_ID "Play Game"
#define EXIT_GAME_BUTTON_ID "Exit"

MainMenu::MainMenu() {
    animation_system = new AnimationSystem();

    buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .42f, .6f, .15f, "MainMenuPlay" ) );
    buttons.push_back( new MenuButton( EXIT_GAME_BUTTON_ID, .5f, .25f, .6f, .15f, "MainMenuExit" ) );
    buttons.push_back( new MenuButton( "logo", .5f, .75f, 1.4f, .35f, "MainMenuChimeraLogo" ) );

    // Add to scene graph for rendering
    for( MenuButton *b : buttons ) {
        scene->addChild(b);
    }

    // Load the level to show in the background
    LevelLoader * level_loader = LevelLoader::loadLevelFile( "Towers" );
    scene->addChild( level_loader->getScene() );
    for(int i = 0; i < level_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(level_loader->getAnimationStack(i));
        level_loader->getAnimationStack(i)->startAllAnimations();
    }
    if(level_loader->getJointList() != nullptr) {
        animation_system->addJointList(level_loader->getJointList());
    }

    // Create a camera to display the background scene
    camera = new Camera();
    scene->addChild( camera );
    camera->setOffset( .1f );
    camera->setAngles( 0.f, -30.f * 3.1415f / 180.f );
    camera->setTransform( glm::vec3(1.f), glm::quat(), glm::vec3(0.f,1.f,0.f) );
    render_system.registerCamera( camera );

    // Add the skybox to the background
    Skybox * skybox = SkyboxFactory::getSkybox("Skyboxes/5Degrees");
    render_system.setSkybox(skybox);
}

MainMenu::~MainMenu() {
    render_system.registerCamera( nullptr );
    scene->setDestroyAll( true );
    delete scene;
    delete animation_system;
}

void MainMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        setNextState( new LoadMenu(), true );
    }
    else if( clicked->getID() == EXIT_GAME_BUTTON_ID ) {
        quit_game = true;
    }
}

void MainMenu::gameLoop() {
    float dt = (float)timer.getLastTickTime();

    camera->modifyAngles( dt/4.f, 0.f );
    camera->createMatrices();

    animation_system->evaluateAnimations(dt);

    render_system.render( 0.f, scene );
}
