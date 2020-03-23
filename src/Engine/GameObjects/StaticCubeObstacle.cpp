#include "StaticCubeObstacle.hpp"

StaticCubeObstacle::StaticCubeObstacle(): GameObject(2) {
    mesh = MeshFactory::createBasicMesh("StaticCube");
    physics = RigidBodyFactory::createCube( identifier, 10.f, 0.f );
    glm::mat4 trans = glm::translate( glm::mat4(1.f), glm::vec3(0.f, -7.f, 0.f) );
    btTransform bt_in;
    bt_in.setFromOpenGLMatrix( &glm::value_ptr(trans)[0] );
    physics->getCollisionObject()->setWorldTransform( bt_in );
}

StaticCubeObstacle::~StaticCubeObstacle() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}
