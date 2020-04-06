#include "InGameState.hpp"

InGameState::InGameState() {
    // // Create the scene and some obstacles
    // scene = new GameObject("0");

    // Obstacle *o1 = new Obstacle();
    // glm::vec3 o1scale(1.f, 1.f, 1.f);
    // glm::quat o1rot( glm::vec3(0.f, 0.f, 0.f) );
    // glm::vec3 o1trans(0.f, 0.f, 0.f);
    // o1->setTransform( o1scale, o1rot, o1trans );

    // Obstacle *o2 = new Obstacle();
    // glm::vec3 o2scale(1.f, 1.f, 1.f);
    // glm::quat o2rot( glm::vec3(0.f, 0.f, 0.f) );
    // glm::vec3 o2trans(2.f, 3.f, 0.f);
    // o2->setTransform( o2scale, o2rot, o2trans );
    
    // Obstacle *o3 = new Obstacle();
    // glm::vec3 o3scale(1.f, 1.f, 1.f);
    // glm::quat o3rot( glm::vec3(glm::radians(12.f), glm::radians(45.f), 0.f) );
    // glm::vec3 o3trans(-1.f, 2.f, 0.f);
    // o3->setTransform( o3scale, o3rot, o3trans );
    
    // StaticCubeObstacle *sc = new StaticCubeObstacle();
    // glm::vec3 scscale(1.f, 1.f, 1.f);
    // glm::quat scrot( glm::vec3(0.f, glm::radians(20.f), glm::radians(45.f)) );
    // glm::vec3 sctrans(0.f, -2.f, 0.f);
    // sc->setTransform( scscale, scrot, sctrans );

<<<<<<< HEAD
    //Set Camera
    camera = new Camera();

    // Add the created obstacles to the screen
    scene->addChild(o1);
    scene->addChild(o2);
    scene->addChild(o3);
    scene->addChild(sc);
=======
    // // Add the created obstacles to the screen
    // scene->addChild(o1);
    // scene->addChild(o2);
    // scene->addChild(o3);
    // scene->addChild(sc);
>>>>>>> master

    physics_system = new PhysicsSystem();
    timer = new StandardTimer();

<<<<<<< HEAD
    //Camera is a child of player so it follows player's movements automatically.
    player->addChild(camera);

    glm::mat4 player_transform = p_trans * p_scale;
    player->setTransform(player_transform);
    // player->setTransform(glm::scale( glm::mat4(1.0), glm::vec3(.01f, .01f, .01f) ));
=======
    scene = LevelLoader::loadLevel("BasicLevel");
>>>>>>> master

    // Create a player and add it to the scene
    Player* player = new Player();
    player->setTransform( glm::vec3(.5f, .5f, .5f), glm::quat(glm::vec3(0.f,0.f,0.f)), glm::vec3(0.f, 1.f, 0.f));
    scene->addChild((GameObject*)player);
    
    playerMovement = new PlayerMovementSystem(player);

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
    int dx = e.motion.xrel;
    int dy = e.motion.yrel;
    // std::cout << "Registered mouse motion with dx, dy: " << dx << ", " << dy << std::endl;
}
