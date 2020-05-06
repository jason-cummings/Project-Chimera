#ifndef SCENERENDERABLE_H
#define SCENERENDERABLE_H

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"

class SceneRenderable: public GameObject {
private:
    Mesh *mesh;

public:
    SceneRenderable( std::string id, Mesh *mesh_in );
    ~SceneRenderable();

    bool hasMesh() const { return true; }
    Mesh * getMesh() const { return mesh; }
};

#endif