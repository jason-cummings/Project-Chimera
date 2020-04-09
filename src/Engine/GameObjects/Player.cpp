#include "Player.hpp"


Player::Player(): GameObject("Player"){
    mesh = MeshFactory::createBasicMesh(std::string("Player"));
}

Player::~Player(){
    delete mesh;
}
