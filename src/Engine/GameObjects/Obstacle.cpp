#include "Obstacle.hpp"

Obstacle::Obstacle(): GameObject(1) {
    mesh = MeshFactory::createBasicMesh("Level");
    physics = nullptr;
}

Obstacle::~Obstacle() {
    delete mesh;
}
