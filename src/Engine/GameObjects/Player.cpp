#include "Player.hpp"


Player::Player(): GameObject(1){
    mesh = MeshFactory::createBasicMesh("Player");
}

Player::~Player(){
    delete mesh;
}
