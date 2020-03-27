#include "StaticCubeObstacle.hpp"

StaticCubeObstacle::StaticCubeObstacle(): GameObject("StaticCube") {
    mesh = MeshFactory::createBasicMesh("StaticCube");
    physics = RigidBodyFactory::createBvhTriangleMeshFromFiles( identifier, "StaticCube" );
}

StaticCubeObstacle::~StaticCubeObstacle() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// Update the transforms of the physics component just before the physics step
void StaticCubeObstacle::setBulletTransforms() const {
    if( physics ) {
        // Convert the worldspace transform and pass it to bullet
        btTransform bt_in;
        bt_in.setFromOpenGLMatrix( &glm::value_ptr(world_transform)[0] );
        btRigidBody * obj = (btRigidBody *)physics->getCollisionObject();
        
        // obj->getMotionState()->setWorldTransform( bt_in );
        obj->setWorldTransform(bt_in);

        // Call the super method to step through children
        GameObject::setBulletTransforms();
    }
}
