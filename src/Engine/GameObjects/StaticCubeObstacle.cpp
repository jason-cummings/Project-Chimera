#include "StaticCubeObstacle.hpp"

StaticCubeObstacle::StaticCubeObstacle(): GameObject(2) {
    mesh = MeshFactory::createBasicMesh("StaticCube");

    btCollisionShape *cube_shape = new btBoxShape( btVector3(5., 5., 5.) );

    btTransform cube_transform;
    cube_transform.setIdentity();
    cube_transform.setOrigin(btVector3(0, -7, 0));

    // Mass of 0 indicates a static object to bullet
    btScalar cube_mass(0.);

    btVector3 local_inertia(0, 0, 0);

    btDefaultMotionState *cube_motion_state = new btDefaultMotionState(cube_transform);
    btRigidBody::btRigidBodyConstructionInfo rb_info(cube_mass, cube_motion_state, cube_shape, local_inertia);
    btRigidBody *body = new btRigidBody(rb_info);

    physics = new PhysicsComponent( identifier, cube_shape, body );
}

StaticCubeObstacle::~StaticCubeObstacle() {
    if( mesh ) delete mesh;
    if( physics ) delete physics;
}

// // Update self and retrieve any necessary data.
// void StaticCubeObstacle::update() {
//     // std::cout << "Static cube" << std::endl;

//     // std::cout << "glm transform: " << transform << std::endl; 
//     if( physics ) {
//         // setTransform( physics->getTransformAsMat4() );
//     }
//     GameObject::update();
// }
