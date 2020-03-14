#include "InGameState.hpp"

InGameState::InGameState() {
    Obstacle *o1 = new Obstacle();

    Obstacle *o2 = new Obstacle();
    glm::mat4 o2trans = glm::scale( glm::translate( o2->getTransform(), glm::vec3(2.f, 3.f, 0.f) ), glm::vec3(.5f, .5f, .5f) );
    o2->setTransform(o2trans);
    
    Obstacle *o3 = new Obstacle();
    glm::mat4 o3trans = glm::scale( glm::rotate( glm::translate( o3->getTransform(), glm::vec3(-1.f, 2.f, 0.f) ), glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f) ), glm::vec3(.25f, .25f, .25f) );
    o3->setTransform(o3trans);

    o1->addChild(o2);
    o2->addChild(o3);
    scene = o1;

    TEMP_th = 0;
    TEMP_ph = 0;

    w = false;
    a = false;
    s = false;
    d = false;
}

void InGameState::update( double dt ) {

    int thdiff = (d-a)*90;
    int phdiff = (s-w)*90;
    TEMP_th += (double)(thdiff+45) * dt;
    TEMP_ph += (double)phdiff * dt;

    glm::mat4 newTrans = glm::rotate( glm::rotate( glm::mat4(1.f), (float)glm::radians(TEMP_ph), glm::vec3(1.f, 0.f, 0.f) ), (float)glm::radians(TEMP_th), glm::vec3(0.f, 1.f, 0.f) );
    scene->setTransform( newTrans );
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
    }
}
