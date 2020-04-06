#ifndef SCENERENDERABLE_H
#define SCENERENDERABLE_H

// #include <glm/gtx/string_cast.hpp>

#include "../GameObject.hpp"
#include "../Systems/Rendering/Mesh.hpp"
#include "../Systems/Rendering/MeshFactory.hpp"

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