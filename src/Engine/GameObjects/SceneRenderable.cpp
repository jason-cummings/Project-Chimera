#include "SceneRenderable.hpp"

SceneRenderable::SceneRenderable( std::string id, Mesh *mesh_in ): GameObject(id) {
    mesh = mesh_in;
}

SceneRenderable::~SceneRenderable() {
    // if( mesh ) delete mesh;
}
