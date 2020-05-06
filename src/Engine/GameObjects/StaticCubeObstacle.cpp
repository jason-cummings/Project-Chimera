#include "StaticCubeObstacle.hpp"

StaticCubeObstacle::StaticCubeObstacle( float scale ): GameObject("StaticCube") {
    mesh = MeshFactory::createBasicMesh(std::string("StaticCube"));
    fs::path cube_path = Asset::assetPath().append("StaticCube");
    btBvhTriangleMeshShape *cube_shape = RigidBodyFactory::createBvhTriangleMeshFromFiles( cube_path );
    glm::vec3 scale_trans(scale);
    physics = RigidBodyFactory::createBvhTriangleMeshComponent( identifier, cube_shape, scale_trans );
}

StaticCubeObstacle::~StaticCubeObstacle() {
    // if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Update the transforms of the physics component just before the physics step
void StaticCubeObstacle::setBulletTransforms() const {
    if( physics ) {
        physics->setTransformationData( bullet_world_transform );
    }

    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
