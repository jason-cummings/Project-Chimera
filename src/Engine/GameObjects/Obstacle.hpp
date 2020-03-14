#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <GL/glew.h>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"

class Obstacle: public GameObject {
private:
    Mesh *mesh;

public:
    Obstacle();
    ~Obstacle();

    bool hasMesh() { return true; }
    Mesh * getMesh() { return mesh; }
};

#endif