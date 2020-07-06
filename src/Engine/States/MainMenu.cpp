#include "MainMenu.hpp"

#include "LevelSelectMenu.hpp"
#include "../LevelLoader.hpp"

#define PLAY_GAME_BUTTON_ID "Play Game"
#define EXIT_GAME_BUTTON_ID "Exit"

#include "../Systems/Rendering/TextManager.hpp"

MainMenu::MainMenu() {
    animation_system = new AnimationSystem();

    // MenuElement *title = new MenuElement( "logo", .5f, .75f, 1.4f, .35f, "MainMenuChimeraLogo" );
    
    TextManager tm( FONT_THERAMIN_GOTHIC_CONDENSED_PATH, 256 );
    GLuint mat;
    unsigned int t_w, t_h;
    tm.createTextTexture("Project Chimera", glm::vec4(1.f, 0.f, 1.f, 1.f), mat, t_w, t_h);
    Material *toUse = new Material( mat, mat, 0 );
    MenuElement *title = new MenuElement( "logo", .5f, .75f, .25f * t_w/(float)t_h, .25f, toUse );
    buttons.push_back( new MenuButton( PLAY_GAME_BUTTON_ID, .5f, .42f, .6f, .15f, "MainMenuPlay" ) );
    buttons.push_back( new MenuButton( EXIT_GAME_BUTTON_ID, .5f, .25f, .6f, .15f, "MainMenuExit" ) );

    // Add to scene graph for rendering
    scene->addChild( title );
    for( MenuButton *b : buttons ) {
        scene->addChild(b);
    }

    // Load the level to show in the background
    LevelLoader * level_loader = LevelLoader::loadLevelFile( "Towers" );
    background_scene = level_loader->getScene();

    for(int i = 0; i < level_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(level_loader->getAnimationStack(i));
        level_loader->getAnimationStack(i)->startAllAnimations();
    }
    if(level_loader->getJointList() != nullptr) {
        animation_system->addJointList(level_loader->getJointList());
    }

    // Create a camera to display the background scene
    camera = new Camera();
    camera->setOffset( .1f );
    camera->setAngles( 0.f, -30.f * 3.1415f / 180.f );
    camera->setTransform( glm::vec3(1.f), glm::quat(), glm::vec3(0.f,1.f,0.f) );
    background_scene->addChild( camera );
    render_system.registerCamera( camera );

    scene->addChild( background_scene );

    // Add the skybox to the background
    Skybox * skybox = SkyboxFactory::getSkybox("Skyboxes/5Degrees");
    render_system.setSkybox(skybox);
}

MainMenu::~MainMenu() {
    // Only delete everything if quitting. Otherwise the level select menu needs it all
    if( quit_game ) {
        delete animation_system;
    }
    else {
        scene->removeChild( background_scene );
    }

    scene->setDestroyAll( true );
    delete scene;
}

void MainMenu::handleButtonEvent( MenuButton *clicked ) {
    if( clicked->getID() == PLAY_GAME_BUTTON_ID ) {
        setNextState( new LevelSelectMenu( background_scene, camera, animation_system ), true );
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

    render_system.render( 0.f );
}
