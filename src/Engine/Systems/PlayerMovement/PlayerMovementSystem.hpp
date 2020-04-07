#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H
#include "./../../GameObjects/Player.hpp"
#include "./../../GameObjects/Camera.hpp"
#include <glm/glm.hpp>
#define MOVE_SPEED 100

class PlayerMovementSystem
{
private: 
    Player* player;
    Camera* camera;
public:
    PlayerMovementSystem( Player* playerptr );
    ~PlayerMovementSystem();
    void movePlayer( int ad, int ss, int ws, double dt );
    inline void registerCamera( Camera* input ) { camera = input; }

};






#endif