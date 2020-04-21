#include "InGameState.hpp"

InGameState::InGameState( std::string level_to_load ) {
    current_level = level_to_load;
    init();
}

void InGameState::init() {
    physics_system = new PhysicsSystem();
    animation_system = new AnimationSystem();
    timer = new StandardTimer();
    camera = new Camera();
    render_system.registerCamera( camera );

    LevelLoader * level_loader = LevelLoader::loadLevelFile(current_level);
    scene = level_loader->getScene();
    for(int i = 0; i < level_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(level_loader->getAnimationStack(i));
    }

    if(level_loader->getJointList() != nullptr) {
        animation_system->addJointList(level_loader->getJointList());
    }
    addPhysicsThings();


    // Create a player and add it to the scene
    // Player* player = new Player();
    // glm::vec3 pscale(1.f);
    // player->setTransform( pscale, glm::quat(glm::vec3(0.f, 0.f, 0.f)), glm::vec3(0.f, 10.f, 0.f) );
    // scene->addChild( player );
    // player->addChild( camera );

    // load the player file and put it into the scene
    LevelLoader * player_loader = LevelLoader::loadCharacterModel();
    GameObject * character_scene = player_loader->getScene();

    for(int i = 0; i < player_loader->getNumAnimationStacks(); i++) {
        animation_system->addAnimationStack(player_loader->getAnimationStack(i));
    }

    if(player_loader->getJointList() != nullptr) {
        animation_system->addJointList(player_loader->getJointList());
    }

    Player * player = (Player *)(character_scene->getGameObject("chimera"));
    player->addChild(camera);

    //add character scene to the level
    for(int i = 0; i < character_scene->getNumChildren(); i++) {
        scene->addChild(character_scene->getChild(i));
    }



    
    playerMovement = new PlayerMovementSystem( physics_system, player, player_loader->getAnimationStack(0) );
    playerMovement->registerCamera( camera );

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

    delete level_loader;
    delete player_loader;
}


void InGameState::addPhysicsThings() {
    glm::vec3 o1scale(.9f);
    glm::quat o1rot( glm::vec3(0.f, glm::radians(0.f), 0.f) );
    glm::vec3 o1trans(0.f, 3.f, 0.f);
    DynamicCube *o1 = new DynamicCube( o1scale.x );
    o1->setTransform( o1scale, o1rot, o1trans );
    
    glm::vec3 o2scale(.9f);
    glm::quat o2rot( glm::vec3(glm::radians(15.f), glm::radians(15.f), 0.f) );
    glm::vec3 o2trans(0.8f, 3.f, 0.f);
    DynamicCube *o2 = new DynamicCube( o2scale.x );
    o2->setTransform( o2scale, o2rot, o2trans );
    
    glm::vec3 o3scale(1.f);
    glm::quat o3rot( glm::vec3(glm::radians(40.f), glm::radians(15.f), glm::radians(25.f)) );
    glm::vec3 o3trans(-1.5f, 4.f, 0.f);
    DynamicCube *o3 = new DynamicCube( o3scale.x );
    o3->setTransform( o3scale, o3rot, o3trans );

    scene->addChild(o1);
    o1->addChild(o2);
    scene->addChild(o3);
}


void InGameState::gameLoop() {
    double dt = timer->getLastTickTime();

    int xmove = int(d-a);
    int ymove = int(space-shift);
    int zmove = int(s-w);
    //sends movement info to PlayerMovementSystem.
    playerMovement->movePlayer( w, s, d, a, space, shift, dt );

    animation_system->evaluateAnimations(dt);

    // Perform necessary updates just before the physics step
    prePhysics();

    // Step physics
    physics_system->stepPhysics(dt);

    // Perform post physics scenegraph updates
    postPhysics();

    // Update the state's scene graph to reflect all changes from the other systems
    // updateScene();

    camera->createMatrices();

    // Render all
    render_system.render( dt, scene );
}

void InGameState::prePhysics() {
    scene->setBulletTransforms();
}

void InGameState::postPhysics() {
    playerMovement->makePostPhysicsAdjustments();

    scene->updateTransformFromPhysics( glm::vec3(1.f), glm::mat4(1.f) );
}

void InGameState::handleKeyDown( SDL_Event e ) {
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
    else if( key == SDLK_SPACE ) {
        space = true;
    }
    else if( key == SDLK_LSHIFT ) {
        shift = true;
    }
}

void InGameState::handleKeyUp( SDL_Event e ) {
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

void InGameState::handleMouseMotion( SDL_Event e ) {
    // For captured mode, get relative mouse motion, not absolute position
    //Scale necessary to scale down movement speed, otherwise blisteringly fast.
    //dx needs to be inverted for proper mouse directional navigation.
    float scale = 0.003;
    float dx = -e.motion.xrel * scale;
    float dy = e.motion.yrel * scale;

    camera->updateCamera(dx,dy);
        // std::cout << "Registered mouse motion with dx, dy: " << dx << ", " << dy << std::endl;
}
