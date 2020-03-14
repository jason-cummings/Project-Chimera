#include "PlayerSubSystem.hpp"

PlayerSubSystem::PlayerSubSystem( Player* playerptr ){
    player = playerptr;
}

PlayerSubSystem::~PlayerSubSystem(){
    delete player;
}

void PlayerSubSystem::movePlayer( int leftRight, int forwardsBackwards, double dt ){
    glm::vec3 movement = glm::vec3( 0.f,0.f,0.f );
    switch ( leftRight )
    {
    case 90:
        //Move Player Right
        movement[0] = MOVE_SPEED;
        break;
    case -90:
        //Move Player Left
        movement[0] = -MOVE_SPEED;
        break;
    }

    switch ( forwardsBackwards )
    {
    case 90:
        //Move Player Backwards
        movement[1] = -MOVE_SPEED;
        break;
    case -90:
        //Move Player Forwards
        movement[1] = MOVE_SPEED;
        break;
    }

    //Moves the player with the given inputs.
    player->setTransform( glm::translate( player->getTransform(), movement*(float)dt ));
}


//Game Object call get and set transforms(takes a matrix, which we get from glm::translate)
//glm::translate(matrix: player->gettransform(),glmvec3: x,y,z)