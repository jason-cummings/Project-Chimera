#include "InGameState.hpp"

InGameState::InGameState( std::string level_to_load ) {
    current_level = level_to_load;
    init();
}

void InGameState::init() {
    physics_system = new PhysicsSystem();
    timer = new StandardTimer();
    camera = new Camera();
    render_system.registerCamera( camera );

    scene = LevelLoader::loadLevel(current_level);
    addPhysicsThings();

    // Create a player and add it to the scene
    Player* player = new Player();
    player->setTransform( glm::vec3(.5f, .5f, .5f), glm::quat(glm::vec3(0.f, 0.f, 0.f)), glm::vec3(100.f, 0.f, 0.f) );
    scene->addChild( player );
    player->addChild( camera );
    
    playerMovement = new PlayerMovementSystem(player);
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
}


void InGameState::addPhysicsThings() {
    glm::vec3 o1scale(20.f);
    glm::quat o1rot( glm::vec3(0.f, 0.f, 0.f) );
    glm::vec3 o1trans(0.f, 0.f, 0.f);
    DynamicCube *o1 = new DynamicCube( o1scale.x );
    o1->setTransform( o1scale, o1rot, o1trans );
    
    glm::vec3 o2scale(30.f);
    glm::quat o2rot( glm::vec3(0.f, glm::radians(25.f), 0.f) );
    glm::vec3 o2trans(-12.f, 40.f, 10.f);
    DynamicCube *o2 = new DynamicCube( o2scale.x );
    o2->setTransform( o2scale, o2rot, o2trans );
    
    glm::vec3 o3scale(15.f);
    glm::quat o3rot( glm::vec3(glm::radians(45.f), glm::radians(15.f), 0.f) );
    glm::vec3 o3trans(20.f, 54.f, 0.f);
    DynamicCube *o3 = new DynamicCube( o3scale.x );
    o3->setTransform( o3scale, o3rot, o3trans );

    glm::vec3 scscale(100.f);
    glm::quat scrot( glm::vec3(0.f) );
    glm::vec3 sctrans(0.f, -100.f, 0.f);
    StaticCubeObstacle *sc = new StaticCubeObstacle( scscale.x );
    sc->setTransform( scscale, scrot, sctrans );


    scene->addChild(o1);
    scene->addChild(o2);
    scene->addChild(o3);
    // scene->addChild(sc);
}


void InGameState::gameLoop() {
    double dt = timer->getLastTickTime();

    // Perform necessary updates just before the physics step
    prePhysics();

    int xmove = int(d-a);
    int ymove = int(space-shift);
    int zmove = int(s-w);
    //sends movement info to PlayerMovementSystem.
    playerMovement->movePlayer( xmove, ymove, zmove, dt );

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
    // This is commented for now because it causes weird time variations in the simulation
    // scene->setBulletTransforms();
}

void InGameState::postPhysics() {
    scene->updateTransformFromPhysics( glm::mat4(1.f) );
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
    float scale = 0.01;
    float dx = -e.motion.xrel * scale;
    float dy = e.motion.yrel * scale;

    camera->updateCamera(dx,dy);
        // std::cout << "Registered mouse motion with dx, dy: " << dx << ", " << dy << std::endl;
}
