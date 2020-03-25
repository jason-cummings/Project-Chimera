#include "Player.hpp"


Player::Player(): GameObject(3){
    mesh = MeshFactory::createBasicMesh("Player");
}

Player::~Player(){
    delete mesh;
}
