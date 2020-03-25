#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H
#include "./../../GameObjects/Player.hpp"
#include <glm/glm.hpp>
#define MOVE_SPEED 45

class PlayerMovementSystem
{
private:
    Player* player;
    //Add Pointer to Camera as well.
public:
    PlayerMovementSystem(Player* playerptr);
    ~PlayerMovementSystem();
    void movePlayer( int xmove, int ymove, int zmove, double dt );

};






#endif