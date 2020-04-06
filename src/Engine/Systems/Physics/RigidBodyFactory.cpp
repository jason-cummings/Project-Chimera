#include "RigidBodyFactory.hpp"

std::map<std::string, btCollisionShape*> RigidBodyFactory::created_collision_shapes;

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
    
    // Create the rigid body and PhysicsComponent to return
    btDefaultMotionState *motion_state = new btDefaultMotionState( default_transform );
    btRigidBody::btRigidBodyConstructionInfo rb_info( bt_mass, motion_state, collision_shape, local_inertia );
    btRigidBody *body = new btRigidBody( rb_info );

    return body;
}

// Attempt to find and return the collision shape in the man, return nullptr if not found
btCollisionShape* RigidBodyFactory::findCollisionShape( std::string name ) {
    std::map<std::string, btCollisionShape*>::iterator it  = created_collision_shapes.find(name);

    if( it != created_collision_shapes.end() ) {
        // Collision shape found, return it
        return it->second;
    }
    else {
        // Not found, return nullptr
        return nullptr;
    }
}

// Create a new cube with the argument attributes
RigidBodyPhysicsComponent * RigidBodyFactory::createCubeComponent( std::string id, float side_length, float mass ) {
    // Test if the cube shape already exists
    std::string cube_shape_name = "_cube_" + std::to_string(side_length);
    btCollisionShape *cube_shape = findCollisionShape( cube_shape_name );

    // If it doesn't exist, create it and add it to the map
    if( cube_shape == nullptr ) {
        float half_box_extent = side_length / 2.f;
        cube_shape = new btBoxShape( btVector3(half_box_extent, half_box_extent, half_box_extent) );
        created_collision_shapes[cube_shape_name] = cube_shape;
    }
    
    // Create the physics component and return it
    RigidBodyPhysicsComponent *ret = createComponent( id, cube_shape, mass );
    return ret;
}

// Create a rigid body from data to be read in under the directory directory_name
// If scale is nullptr, don't scale, else scale must be a 3 float array
btBvhTriangleMeshShape * RigidBodyFactory::createBvhTriangleMeshFromFiles( fs::path directory ) {
    // Test if this shape already exsts
    std::string mesh_shape_name = directory.filename().string();
    btBvhTriangleMeshShape *mesh_shape = (btBvhTriangleMeshShape*)findCollisionShape( mesh_shape_name );

    // If it doesn't exist, create it and add it to the map
    if( mesh_shape == nullptr ) {
        // Read in the files for the vertex and index data
        fs::path vbo_path = directory;
        vbo_path.append("VBO");
        fs::path ibo_path = directory;
        ibo_path.append("IBO");
        Asset vertices( vbo_path );
        Asset indices( ibo_path );

        // Get the necessary data to create the 
        // Create an indexed mesh with all of the read in data
        btIndexedMesh indexed_mesh;
        indexed_mesh.m_numTriangles =         indices.getBytes() / sizeof(int) / 3; // 3 indices for a triangle
        indexed_mesh.m_triangleIndexBase =    (const unsigned char*)(indices.copyBuffer());
        indexed_mesh.m_triangleIndexStride =  3 * sizeof(int);
        indexed_mesh.m_numVertices =          vertices.getBytes() / (19 * sizeof(float)); // 19 data points per vertex
        indexed_mesh.m_vertexBase =           (const unsigned char*)(vertices.copyBuffer());
        indexed_mesh.m_vertexStride =         19 * sizeof(float); // 19 data points per vertex

        // Create a triangle vertex array with the indexed mesh
        btTriangleIndexVertexArray *mesh_data = new btTriangleIndexVertexArray();
        mesh_data->addIndexedMesh( indexed_mesh );

        // Create a collision shape with the data and store it
        mesh_shape = new btBvhTriangleMeshShape( mesh_data, true );
        created_collision_shapes[mesh_shape_name] = mesh_shape;
    }
    
    return mesh_shape;
}

RigidBodyPhysicsComponent * RigidBodyFactory::createBvhTriangleMeshComponent( std::string id, btBvhTriangleMeshShape *collision_shape, glm::vec3 scale ) {
    // Create a scaled bvh mesh if necessary
    btCollisionShape *use_shape = collision_shape;
    if( scale.x != 1.f || scale.x != 1.f || scale.x != 1.f ) {
        btVector3 bt_scale( scale.x, scale.y, scale.z );
        use_shape = new btScaledBvhTriangleMeshShape( collision_shape, bt_scale );
    }

    btRigidBody *body = createRigidBody( use_shape, 0.f );
    RigidBodyPhysicsComponent *ret = new RigidBodyPhysicsComponent( id, use_shape, body );
    return ret;
}

// Create the actual btRigidBody object from the collision object and mass
RigidBodyPhysicsComponent * RigidBodyFactory::createComponent( std::string id, btCollisionShape *collision_shape, float mass ) {
    btRigidBody *body = createRigidBody( collision_shape, mass );
    RigidBodyPhysicsComponent *ret = new RigidBodyPhysicsComponent( id, collision_shape, body );
    return ret;
}
