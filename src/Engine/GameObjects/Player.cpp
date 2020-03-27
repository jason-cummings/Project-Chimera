#include "Player.hpp"


Player::Player(): GameObject("Player"){
    mesh = MeshFactory::createBasicMesh("Player");
}

Player::~Player(){
    delete mesh;
}
