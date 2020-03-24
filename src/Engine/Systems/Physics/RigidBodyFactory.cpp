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

// Create the actual btRigidBody object from the collision object and mass
btRigidBody * RigidBodyFactory::createRigidBody( btCollisionShape *collision_shape, float mass ) {
    // Create a default transform at the origin that can be updated later
    btTransform default_transform;
    default_transform.setIdentity();
    default_transform.setOrigin( btVector3(0, 0, 0) );

    // Objects are dynamic iff mass != 0
    btScalar bt_mass( mass );
    bool is_dynamic = (mass != 0.f);
    btVector3 local_inertia(0, 0, 0);
    if( is_dynamic ) {
        collision_shape->calculateLocalInertia( bt_mass, local_inertia );
    }
    
    // Create the rigid body
    btDefaultMotionState *motion_state = new btDefaultMotionState( default_transform );
    btRigidBody::btRigidBodyConstructionInfo rb_info( bt_mass, motion_state, collision_shape, local_inertia );
    btRigidBody *body = new btRigidBody( rb_info );

    return body;
}

// Create a new cube with the argument attributes
RigidBodyPhysicsComponent * RigidBodyFactory::createCube( int id, float side_length, float mass ) {
    // Test if the cube shape already exists
    std::string cube_shape_name = "_cube_" + std::to_string(side_length);
    btCollisionShape *cube_shape = findCollisionShape( cube_shape_name );

    // If it doesn't exist, create it and add it to the map
    if( cube_shape == nullptr ) {
        float half_box_extent = side_length / 2.f;
        cube_shape = new btBoxShape( btVector3(half_box_extent, half_box_extent, half_box_extent) );
        collision_shapes[cube_shape_name] = cube_shape;
    }
    
    // Create the physics component and return it
    btRigidBody *body = createRigidBody( cube_shape, mass );
    RigidBodyPhysicsComponent *ret = new RigidBodyPhysicsComponent( id, cube_shape, body );
    return ret;
}

// Create a rigid body from data to be read in under the directory directory_name
RigidBodyPhysicsComponent * RigidBodyFactory::createBvhTriangleMeshFromFiles( int id, std::string directory_name ) {
    // Test if this shape already exsts
    std::string mesh_shape_name = "_bvh_triangle_mesh_" + directory_name;
    btCollisionShape *mesh_shape = findCollisionShape( mesh_shape_name );

    // If it doesn't exist, create it and add it to the map
    if( mesh_shape == nullptr ) {
        // Read in the files for the vertex and index data
        Asset vertices( directory_name + "/VBO" );
        Asset indices( directory_name + "/IBO" );

        // Get the necessary data to create the 
        int num_vertices = indices.getBytes() / sizeof(int);
        int num_triangles = num_vertices / 3;

        // Create an indexed mesh with all of the read in data
        btIndexedMesh indexed_mesh;
        indexed_mesh.m_numTriangles = num_triangles;
        indexed_mesh.m_triangleIndexBase = (const unsigned char*)(indices.copyBuffer());
        indexed_mesh.m_triangleIndexStride = 3 * sizeof(int);
        indexed_mesh.m_numVertices = num_vertices;
        indexed_mesh.m_vertexBase = (const unsigned char*)(vertices.copyBuffer());
        indexed_mesh.m_vertexStride = 19*sizeof(float); // Ehhhhhh this might work (seems to anyways)

        // Create a triangle vertex array with the indexed mesh
        btTriangleIndexVertexArray *mesh_data = new btTriangleIndexVertexArray();
        mesh_data->addIndexedMesh( indexed_mesh );

        // Create a collision shape with the data and store it
        mesh_shape = new btBvhTriangleMeshShape( mesh_data, true );
        collision_shapes[mesh_shape_name] = mesh_shape;
    }
    
    // Create the physics component and return it
    btRigidBody *body = createRigidBody( mesh_shape, 0.f );
    RigidBodyPhysicsComponent *ret = new RigidBodyPhysicsComponent( id, mesh_shape, body );
    return ret;
}
