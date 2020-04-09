#include "PlayerMovementSystem.hpp"

PlayerMovementSystem::PlayerMovementSystem( Player* playerptr ){
    player = playerptr;
}

PlayerMovementSystem::~PlayerMovementSystem(){
    delete player;
}

void PlayerMovementSystem::movePlayer( int ad, int ss, int ws, double dt ){
    double th = camera->getTh();   
    while(th > 6.28){
        th -= 6.28;
    }
    while(th < -6.28){
        th += 6.28;
    }

    double movex = MOVE_SPEED * (ws * sin(th) + ad * cos(th))/2;
    double movey = MOVE_SPEED * (double)ss;
    double movez = MOVE_SPEED * (ws * cos(th) + ad * -sin(th))/2;
    
    glm::vec3 movement = glm::vec3( movex, movey, movez ) * (float)dt;

    //Moves the player with the given inputs.
    player->setTranslation(  player->getTranslation() + movement );
}


//Game Object call get and set transforms(takes a matrix, which we get from glm::translate)
//glm::translate(matrix: player->gettransform(),glmvec3: x,y,z)