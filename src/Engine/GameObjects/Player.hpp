#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "./../Systems/Rendering/Mesh.hpp"
#include "./../Systems/Rendering/MeshFactory.hpp"
#include "./../GameObject.hpp"

class Player : public GameObject
{
private:
Mesh* mesh;
    
public:
    Player();
    ~Player();

    bool hasMesh() const { return true; }
	Mesh * getMesh() const { return mesh; }
};






#endif