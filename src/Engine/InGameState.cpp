#include "InGameState.hpp"

InGameState::InGameState() {
    // Create the scene and some obstacles
    scene = new GameObject(0);

    Obstacle *o1 = new Obstacle();
    o1->setTransform( glm::scale( glm::translate( glm::mat4(1.0), glm::vec3(0.f, 0.f, 0.f) ), glm::vec3(1.f, 1.f, 1.f) ) );

    Obstacle *o2 = new Obstacle();
    glm::mat4 o2trans = glm::scale( glm::translate( o2->getTransform(), glm::vec3(2.f, 3.f, 0.f) ), glm::vec3(1.f, 1.f, 1.f) );
    o2->setTransform(o2trans);
    
    Obstacle *o3 = new Obstacle();
    glm::mat4 o3trans = glm::scale( glm::rotate( glm::translate( o3->getTransform(), glm::vec3(-1.f, 2.f, 0.f) ), glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f) ), glm::vec3(1.f, 1.f, 1.f) );
    o3->setTransform(o3trans);
    
    StaticCubeObstacle *sc = new StaticCubeObstacle();
    glm::mat4 sctrans = glm::scale( glm::translate( sc->getTransform(), glm::vec3(0.f, -2.f, 0.f) ), glm::vec3(1.f, 1.f, 1.f) );
    sc->setTransform(sctrans);

    // Add the created obstacles to the screen
    scene->addChild(o1);
    scene->addChild(o2);
    scene->addChild(o3);
    scene->addChild(sc);

    // Create a player and add it to the scene
    Player* player = new Player();
    glm::mat4 p_scale = glm::scale(glm::mat4(1.f), glm::vec3(.01f, .01f, .01f));
    glm::mat4 p_trans = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f));

    glm::mat4 player_transform = p_trans * p_scale;
    player->setTransform(player_transform);
    // player->setTransform(glm::scale( glm::mat4(1.0), glm::vec3(.01f, .01f, .01f) ));

    scene->addChild((GameObject*)player);
    playerMovement = new PlayerMovementSystem(player);

    scene->setBulletTransforms();
    
    // Initialize keyboard controls variables
    w = false;
    a = false;
    s = false;
    d = false;
    shift = false;
    space = false;
}

void InGameState::update( double dt ) {
    int xmove = int(d-a);
    int ymove = int(space-shift);
    int zmove = int(s-w);
    //sends movement info to PlayerMovementSystem.
    playerMovement->movePlayer( xmove, ymove, zmove, dt );
}

void InGameState::prePhysics() {
    // This is commented for now because it causes weird time variations in the simulation
    // scene->setBulletTransforms();
}

void InGameState::postPhysics() {
    scene->updateTransformFromPhysics( glm::mat4(1.f) );
}

void InGameState::handleSDLEvent( SDL_Event e ) {
    if( e.type == SDL_KEYDOWN ) {
        // Handle any keydown events
        SDL_Keycode inputValue = e.key.keysym.sym;
        if(inputValue == SDLK_w){
            w = true;
        }
        else if(inputValue == SDLK_s){
            s = true;
        }
        else if(inputValue == SDLK_a){
            a = true;
        }
        else if(inputValue == SDLK_d){
            d = true;
        }
        else if(inputValue == SDLK_SPACE){
            space = true;
        }
        else if(inputValue == SDLK_LSHIFT){
            shift = true;
        }
    }
    else if( e.type == SDL_KEYUP ) {
        // Handle any keyup events
        SDL_Keycode inputValue = e.key.keysym.sym;
        if(inputValue == SDLK_w){
            w = false;
        }
        else if(inputValue == SDLK_s){
            s = false;
        }
        else if(inputValue == SDLK_a){
            a = false;
        }
        else if(inputValue == SDLK_d){
            d = false;
        }
        else if(inputValue == SDLK_SPACE){
            space = false;
        }
        else if(inputValue == SDLK_LSHIFT){
            shift = false;
        }
    }
}
