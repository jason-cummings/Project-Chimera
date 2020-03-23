#include "RigidBodyFactory.hpp"

std::map<std::string, btCollisionShape*> RigidBodyFactory::collision_shapes;

// Attempt to find and return the collision shape in the man, return nullptr if not found
btCollisionShape* RigidBodyFactory::findCollisionShape( std::string name ) {
    std::map<std::string, btCollisionShape*>::iterator it  = collision_shapes.find(name);

    if( it != collision_shapes.end() ) {
        // Collision shape found, return it
        return it->second;
    }
    else {
        // Not found, return nullptr
        return nullptr;
    }
}

// Create a new cube with the argument attributes
RigidBodyPhysicsComponent * RigidBodyFactory::createCube( int id, float side_length, float mass ) {
    // Test if the cube shape already exists
    std::string cube_shape_name = "_cube" + std::to_string(side_length);
    btCollisionShape *cube_shape = findCollisionShape( cube_shape_name );

    // If it doesn't exist, create it and add it to the map
    if( cube_shape == nullptr ) {
        float half_box_extent = side_length / 2.f;
        cube_shape = new btBoxShape( btVector3(half_box_extent, half_box_extent, half_box_extent) );
        collision_shapes[cube_shape_name] = cube_shape;
    }
    
    // Create a default transform at the origin that can be updated later
    btTransform cube_transform;
    cube_transform.setIdentity();
    cube_transform.setOrigin( btVector3(0, 0, 0) );

    // Objects are dynamic iff mass != 0
    btScalar cube_mass( mass );
    bool is_dynamic = (mass != 0.f);
    btVector3 local_inertia(0, 0, 0);
    if( is_dynamic ) {
        cube_shape->calculateLocalInertia( cube_mass, local_inertia );
    }
    
    // Create the rigid body
    btDefaultMotionState *cube_motion_state = new btDefaultMotionState( cube_transform );
    btRigidBody::btRigidBodyConstructionInfo rb_info( cube_mass, cube_motion_state, cube_shape, local_inertia );
    btRigidBody *body = new btRigidBody( rb_info );

    // Create the physics component and return it
    RigidBodyPhysicsComponent *ret = new RigidBodyPhysicsComponent( id, cube_shape, body );
    return ret;
}
