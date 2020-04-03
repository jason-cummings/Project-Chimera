#include "PlayerMovementSystem.hpp"

PlayerMovementSystem::PlayerMovementSystem( Player* playerptr ){
    player = playerptr;
}

PlayerMovementSystem::~PlayerMovementSystem(){
    delete player;
}

void PlayerMovementSystem::movePlayer( int xmove, int ymove, int zmove, double dt ){
    glm::vec3 movement = glm::vec3( 
                            (double)xmove * MOVE_SPEED,     // x movement
                            (double)ymove * MOVE_SPEED,     // y movement
                            (double)zmove * MOVE_SPEED      // z movement
    ) * (float)dt;

    //Moves the player with the given inputs.
    player->setTranslation(  player->getTranslation() + movement );
}


//Game Object call get and set transforms(takes a matrix, which we get from glm::translate)
//glm::translate(matrix: player->gettransform(),glmvec3: x,y,z)