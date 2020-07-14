#include "InGameState.hpp"

#include "MainMenu.hpp"
#include "PauseMenu.hpp"
#include "WinMenu.hpp"

// #include "../GameObjects/Obstacle.hpp"
#include "../GameObjects/DynamicCube.hpp"
// #include "../GameObjects/StaticCubeObstacle.hpp"
#include "../GameObjects/Player.hpp"

#include "../LevelLoader.hpp"

#include "../SettingsManager.hpp"

InGameState::InGameState( std::string level_to_load ) {
    current_level = level_to_load;
    init();
}

InGameState::~InGameState() {
    delete physics_system;
    delete animation_system;

    if( camera == render_system.getRegisteredCamera() ) render_system.registerCamera( nullptr );
    scene->setDestroyAll(true);
    delete scene;
}

void InGameState::init() {
    physics_system = new PhysicsSystem();
    animation_system = new AnimationSystem();
    camera = new Camera();
    camera->setOffset( 3.f );
    camera->setResolution( UserSettings::resolution_width, UserSettings::resolution_height );
    render_system.registerCamera( camera );

    mouse_lock = true;
    player_visible = true;

    LevelLoader * level_loader = LevelLoader::loadLevelFile(current_level);
    scene = level_loader->getScene();
    for(int i = 0; i < level_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(level_loader->getAnimationStack(i));
        level_loader->getAnimationStack(i)->startAllAnimations();
    }

    if(level_loader->getJointList() != nullptr) {
        animation_system->addJointList(level_loader->getJointList());
    }

    end_coords = level_loader->getEndGameCoords();
    addPhysicsThings();

    // load the player file and put it into the scene
    LevelLoader * player_loader = LevelLoader::loadCharacterModel();
    GameObject * character_scene = player_loader->getScene();

    for(int i = 0; i < player_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(player_loader->getAnimationStack(i));
    }

    if(player_loader->getJointList() != nullptr) {
        animation_system->addJointList(player_loader->getJointList());
    }

    player = (Player *)(character_scene->getGameObject("chimera"));
    player->addChild(camera);

    //add character scene to the level
    for(int i = 0; i < character_scene->getNumChildren(); i++) {
        scene->addChild(character_scene->getChild(i));
    }

    player_movement = new PlayerMovementSystem( physics_system, player, player_loader->getAnimationStack(0) );
    player_movement->registerCamera( camera );

    // Add the scene graph to Bullet and set the transforms appropriately
    physics_system->addSceneComponents( scene );
    scene->setBulletTransforms();

    // Initialize keyboard controls variables
    w = false;
    a = false;
    s = false;
    d = false;
    shift = false;
    space = false;

    first_tick = true;

    delete level_loader;
    delete player_loader;

    Skybox * skybox = SkyboxFactory::getSkybox("Skyboxes/5Degrees");
    render_system.setSkybox(skybox);
}


void InGameState::addPhysicsThings() {
    glm::vec3 o1scale(.9f);
    glm::quat o1rot( glm::vec3(0.f, glm::radians(0.f), 0.f) );
    glm::vec3 o1trans(0.f, 3.f, 10.f);
    DynamicCube *o1 = new DynamicCube( o1scale.x );
    o1->setTransform( o1scale, o1rot, o1trans );
    
    glm::vec3 o2scale(.9f);
    glm::quat o2rot( glm::vec3(glm::radians(15.f), glm::radians(15.f), 0.f) );
    glm::vec3 o2trans(0.8f, 3.f, 10.f);
    DynamicCube *o2 = new DynamicCube( o2scale.x );
    o2->setTransform( o2scale, o2rot, o2trans );
    
    glm::vec3 o3scale(1.f);
    glm::quat o3rot( glm::vec3(glm::radians(40.f), glm::radians(15.f), glm::radians(25.f)) );
    glm::vec3 o3trans(-1.5f, 4.f, 10.f);
    DynamicCube *o3 = new DynamicCube( o3scale.x );
    o3->setTransform( o3scale, o3rot, o3trans );

    scene->addChild(o1);
    scene->addChild(o2);
    scene->addChild(o3);
}

void InGameState::togglePlayerVisibility() {
    player_visible = !player_visible;
    if( player_visible ) {
        render_system.populateRenderLists( player );
    }
    else {
        render_system.removeGameObjectFromRenderListsRecursive( player );
    }
}

void InGameState::gameLoop() {
    double dt = timer.getLastTickTime();

    performance_logger.startTick();

    if( first_tick ) {
        dt = 0;
        first_tick = false;
    }
    
    if( fell() ){
        glm::vec3 spawnPoint = glm::vec3(0.f,10.f,0.f);
        player->setTranslation(spawnPoint);
    }

    //If player reaches end goal
    if( endGame() ){
        scene->setDestroyAll( true );
        setNextState( new WinMenu(this), false );
    }

    //sends movement info to Player_movementSystem.
    player_movement->updatePlayerMovement( w, s, d, a, space, shift, dt );

    performance_logger.addOperation("Player Movement",timer.timePerformance());

    animation_system->evaluateAnimations(dt);

    performance_logger.addOperation("Animation",timer.timePerformance());

    // Perform necessary updates just before the physics step
    prePhysics();

    // Step physics
    physics_system->stepPhysics(dt);

    // Perform post physics scenegraph updates
    postPhysics();

    performance_logger.addOperation("Physics",timer.timePerformance());

    camera->createMatrices();

    // Render all
    render_system.render( dt );

    performance_logger.addOperation("Render",timer.timePerformance());
    performance_logger.stopTick();
}

/**
 * Copies scenegraph world transforms to Bullet
 */
void InGameState::prePhysics() {
    scene->setBulletTransforms();
}

/**
 * Copies transforms back from Bullet after the physics step
 */
void InGameState::postPhysics() {
    player_movement->makePostPhysicsAdjustments();
    scene->updateTransformFromPhysics( glm::vec3(1.f), glm::mat4(1.f) );
}

void InGameState::handleKeyDownStateSpecific( SDL_Event e ) {
    SDL_Keycode key = e.key.keysym.sym;
    if( key == SDLK_w ) {
        w = true;
    }
    else if( key == SDLK_s ) {
        s = true;
    }
    else if( key == SDLK_a ) {
        a = true;
    }
    else if( key == SDLK_d ) {
        d = true;
    }
    else if( key == SDLK_f ) {
        player_movement->toggleFlying();
    }
    else if( key == SDLK_g ) {
        togglePlayerVisibility();
    }
    else if( key == SDLK_SPACE ) {
        space = true;
    }
    else if( key == SDLK_LSHIFT ) {
        shift = true;
    }
    else if( key == SDLK_ESCAPE ) {
        movementFalse();
        setNextState( new PauseMenu(this), false );
        first_tick = true;
    }
    else if( key == SDLK_F3 ) {
        //Prints out coordinates in terminal
        glm::vec4 tempo = player->getWorldTransform()[3];
        std::cout << "COORDS -> " << std::endl;
        std::cout << "x -> "  << tempo[0] <<std::endl;
        std::cout << "y -> " << tempo[1]<<std::endl;
        std::cout << "z -> " << tempo[2]<<std::endl;
    }
}

void InGameState::handleKeyUpStateSpecific( SDL_Event e ) {
    SDL_Keycode key = e.key.keysym.sym;
    if( key == SDLK_w ) {
        w = false;
    }
    else if( key == SDLK_s ) {
        s = false;
    }
    else if( key == SDLK_a ) {
        a = false;
    }
    else if( key == SDLK_d ) {
        d = false;
    }
    else if( key == SDLK_SPACE ) {
        space = false;
    }
    else if( key == SDLK_LSHIFT ) {
        shift = false;
    }
}

void InGameState::handleMouseMotionStateSpecific( SDL_Event e ) {
    // For captured mode, get relative mouse motion, not absolute position
    //Scale necessary to scale down movement speed, otherwise blisteringly fast.
    //dx needs to be inverted for proper mouse directional navigation.
    float scale = 0.003f;
    float dx = -e.motion.xrel * scale;
    float dy = e.motion.yrel * scale;

    camera->modifyAngles(dx,dy);
        // std::cout << "Registered mouse motion with dx, dy: " << dx << ", " << dy << std::endl;
}

bool InGameState::isNear(float input, float goal){
    float threshold = 3.f;
    return abs(goal - input) < threshold;
}

//If Player falls more than 30m below spawn point
bool InGameState::fell(){
    return (player->getWorldTransform()[3][1] < -30);
}


bool InGameState::endGame() {
    glm::vec4 coords = player->getWorldTransform()[3];
    float x,y,z;
    x = coords[0];
    y = coords[1];
    z = coords[2];
    return (isNear( x, end_coords[0]) && isNear(y , end_coords[1]) && isNear( z, end_coords[2]));
}

void InGameState::movementFalse(){
    w = false;
    a = false;
    s = false;
    d = false;
    shift = false;
    space = false;
}
