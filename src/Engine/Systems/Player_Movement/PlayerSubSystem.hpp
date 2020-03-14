#ifndef PLAYER_SUBSYSTEM_HPP
#define PLAYER_SUBSYSTEM_HPP
#include "./../../GameObjects/Player.hpp"
#include <glm/glm.hpp>
#define MOVE_SPEED 15

class PlayerSubSystem
{
private:
    Player* player;
    //Add Pointer to Camera as well.
public:
    PlayerSubSystem(Player* playerptr);
    ~PlayerSubSystem();
    void movePlayer(int leftRight, int forwardsBackwards, double dt);

};






#endif