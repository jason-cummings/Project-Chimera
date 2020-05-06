#include "SceneRenderable.hpp"

// #include <glm/gtx/string_cast.hpp>

#include "../Systems/Rendering/MeshFactory.hpp"

SceneRenderable::SceneRenderable( std::string id, Mesh *mesh_in ): GameObject(id) {
    mesh = mesh_in;
}

SceneRenderable::~SceneRenderable() {
    // if( mesh ) delete mesh;
}
