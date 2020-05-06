#include "DynamicCube.hpp"

int DynamicCube::num_cubes = 0;

DynamicCube::DynamicCube( float scale ): GameObject("DynamicCube" + std::to_string(++num_cubes)) {
    mesh = MeshFactory::createBasicMesh(std::string("DynamicCube"));
    fs::path mat_path = Asset::assetPath();
    mat_path.append("Materials/LoadingScreen");
    Material *use_mat = MaterialFactory::createMaterial(mat_path);
    mesh->setMaterial(use_mat);
    physics = RigidBodyFactory::createCubeComponent( identifier, scale, pow( scale, 3 )*100.f );
}

DynamicCube::~DynamicCube() {
    // if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Get the transform from the physics component 
void DynamicCube::updateTransformFromPhysics( glm::vec3 parent_scale, glm::mat4 parent_bullet_transfrom ) {
    // Get the updated transformation data from physics
    if( physics ) {
        glm::vec3 new_translation;
        glm::quat new_rotation;
        physics->getTransformationData( parent_scale, parent_bullet_transfrom, new_translation, new_rotation );
        setTransform( scale, new_rotation, new_translation );
    }

    // Call super method to step through children
    GameObject::updateTransformFromPhysics( scale, bullet_world_transform );
}

// Update the transforms of the physics component just before the physics step
void DynamicCube::setBulletTransforms() const {
    if( physics ) {
        physics->setTransformationData( bullet_world_transform );
    }

    // Call the super method to step through children
    GameObject::setBulletTransforms();
}
