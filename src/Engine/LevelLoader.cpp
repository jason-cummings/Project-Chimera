#include "LevelLoader.hpp"

#define DEBUG_LOADER

// get a level loader for getting the character model
LevelLoader * LevelLoader::loadCharacterModel() {
    LevelLoader * level_loader = new LevelLoader(PLAYER_MODEL_DIRECTORY);
    return level_loader;
}
// get a level loader for loading standard levels
LevelLoader * LevelLoader::loadLevelFile(std::string name) {
    LevelLoader * level_loader = new LevelLoader(LEVELS_DIRECTORY + name);
    return level_loader;
}


// Calls loadLevel with the passed in level name
LevelLoader::LevelLoader( std::string level_name ) {
    scene = nullptr;
    joint_list = nullptr;
    createLevel(level_name);
}

// Delete any stored info
LevelLoader::~LevelLoader() {

}

// Get the root path of the level
fs::path LevelLoader::levelPath( std::string level_name ) const {
    return (fs::path)(WAIWrapper::getExecutablePath() +  "/" + level_name);
}

// Wrapper for appending to an fs::path
fs::path LevelLoader::pathAppend( fs::path in, std::string to_append ) const {
    fs::path ret = in;
    ret.append( to_append );
    return ret;
}

// Wrapper for reading a property file as an std::string. Returns empty string if property not opened
std::string LevelLoader::getPropertyContents( fs::path property_path ) const {
    std::string ret = "";
    if( fs::exists( property_path ) ) {
        Asset obj_mesh_asset( property_path );
        ret = std::string( obj_mesh_asset.getBuffer() );
    }
    return ret;
}

void LevelLoader::createLevel( std::string level_name ) {
    // Ensure that the level can be opened
    fs::path level_path = levelPath(level_name);

    if( !fs::exists(level_path) ) {
        std::cerr << "Could not open level " << level_name << std::endl;
        std::cerr << "Directory at path " << level_path << " could not be opened" << std::endl;
        return;
    }

    // Get the directory of the root node and parse it recursively
    fs::path scene_path = level_path;
    scene_path.append( LEVEL_ROOTNODE_DNAME );
    LoadedObjectProperties *scene_properties = parseObjectDirectory( LEVEL_ROOTNODE_DNAME, scene_path );

    //Load all the level materials
    fs::path materials_path = level_path;
    materials_path.append( LEVEL_MATERIALS_DNAME );
    fs::path textures_path = level_path;
    textures_path.append( LEVEL_TEXTURES_DNAME );
    loadMaterials( materials_path, textures_path );

    // Load all the level objects
    fs::path mesh_path = level_path;
    mesh_path.append( LEVEL_MESH_DNAME );
    loadMeshes( mesh_path );

    //load joints from level
    fs::path joint_path = level_path;
    joint_path.append("JointList");
    loadJointList(joint_path);

    // Load all skinned mesh objects
    fs::path skinned_mesh_path = level_path;
    skinned_mesh_path.append(LEVEL_SKINNED_MESH_DNAME);
    loadSkinnedMeshes( skinned_mesh_path );

    // Load all the level objects
    fs::path colshapes_path = level_path;
    colshapes_path.append( LEVEL_COLLISION_SHAPES_DNAME );
    loadCollisionShapes( colshapes_path );

    // Create the scene with the properties read in
    createScene( scene_properties );

    scene->print();

    // load animations
    fs::path animation_path = level_path;
    animation_path.append( LEVEL_ANIMATION_DNAME );
    loadAnimations(animation_path);

    //load endgame coordinates
    fs::path endgame_path = level_path;
    endgame_path.append( LEVEL_ENDGAME_DNAME );
    loadEndGame( endgame_path );

    //set pointers for joint objects as they need pointers to gameobjects
    if(joint_list) {
        joint_list->setJointPointers(scene);
        joint_list->calculateInverseBindPose();
    }
    


    delete scene_properties;
}

// Load information for a single object, then call recursively on object's children
LoadedObjectProperties * LevelLoader::parseObjectDirectory( std::string object_name, fs::path object_path ) const {
    LoadedObjectProperties *new_obj = new LoadedObjectProperties();

    // Make sure the object exists
    if( !fs::exists(object_path) ) {
        std::cerr << "Could not open game object directory " << object_name << std::endl;
        std::cerr << "Directory at path " << object_path << " could not be opened" << std::endl;
        return new_obj;
    }
    new_obj->identifier = object_name;
    
    // Look for any possible relevant property paths
    fs::path obj_mesh_path =            pathAppend( object_path, OBJECT_MESH_FNAME );
    fs::path obj_skinned_mesh_path =    pathAppend( object_path, OBJECT_SKINNED_MESH_FNAME );
    // fs::path obj_material_path =        pathAppend( object_path, OBJECT_MATERIAL_FNAME );

    fs::path obj_collision_shape_path = pathAppend( object_path, OBJECT_COLLISION_SHAPE_FNAME );
    fs::path obj_rotation_path =        pathAppend( object_path, OBJECT_ROTATION_FNAME );
    fs::path obj_scaling_path =         pathAppend( object_path, OBJECT_SCALING_FNAME );
    fs::path obj_translation_path =     pathAppend( object_path, OBJECT_TRANSLATION_FNAME );
    fs::path obj_children_path =        pathAppend( object_path, OBJECT_CHILDREN_DNAME );
    fs::path obj_bone_path =            pathAppend( object_path, OBJECT_BONE_FNAME );

    // Read in the property indicators
    new_obj->mesh_id =                  getPropertyContents( obj_mesh_path );
    new_obj->skinned_mesh_id =          getPropertyContents( obj_skinned_mesh_path );
    // new_obj->material_id =              getPropertyContents( obj_material_path );
    new_obj->collision_shape_id =       getPropertyContents( obj_collision_shape_path );



    new_obj->is_bone =                  getPropertyContents( obj_bone_path ) != std::string("");

    // Read in asset files for the transformation data
    Asset obj_rotation_asset( obj_rotation_path );
    Asset obj_scaling_asset( obj_scaling_path );
    Asset obj_translation_asset( obj_translation_path );

    // Copy the data to the appropriate object properties
    float *rotdata = (float *)obj_rotation_asset.copyBuffer();
    float *scaledata = (float *)obj_scaling_asset.copyBuffer();
    float *transdata = (float *)obj_translation_asset.copyBuffer();

    new_obj->rotation = glm::vec3( rotdata[0], rotdata[1], rotdata[2] );
    new_obj->scaling = glm::vec3( scaledata[0], scaledata[1], scaledata[2] );
    new_obj->translation = glm::vec3( transdata[0], transdata[1], transdata[2] );

    // Loop over the children and populate the current object's children
    if( fs::exists(obj_children_path) ) {
        for( auto& child_dir: fs::directory_iterator(obj_children_path) ) {
            if( fswrapper::is_dir( child_dir ) ) {
                // Get the name and the path
                fs::path child_path = child_dir.path();
                std::string child_name = child_path.filename().string();

                // Create the new child and add it
                LoadedObjectProperties *new_child = parseObjectDirectory( child_name, child_path );
                new_obj->children.push_back( new_child );
            }
        }
    }

    return new_obj;
}

void LevelLoader::loadMeshes( fs::path dir ) {
    if( fs::exists(dir) ) {
        // Loop through all folders in the mesh dir and added them to the loaded_meshes
        for( auto& mesh_dir: fs::directory_iterator(dir) ) {
            // Create the mesh from the found VBO and IBO
            fs::path mesh_path = mesh_dir.path();
            Mesh* created_mesh = MeshFactory::createBasicMesh( mesh_path );
            
            // Test if the mesh has a material
            fs::path material_path = mesh_dir.path();
            material_path.append( OBJECT_MATERIAL_FNAME );
            if( fs::exists( material_path ) ) {
                // Use the found material
                std::string material_name = getPropertyContents( material_path );
                created_mesh->setMaterial( loaded_materials[material_name] );
            }
            loaded_meshes[mesh_path.filename().string()] = created_mesh;
        }
    }
}

void LevelLoader::loadSkinnedMeshes( fs::path dir ) {
    // Loop through all folders in the mesh dir and added them to the loaded_meshes
    if( fs::exists(dir) ) {
        for( auto& skinned_mesh_dir: fs::directory_iterator(dir) ) {
            // Create the mesh from the found VBO and IBO
            fs::path skinned_mesh_path = skinned_mesh_dir.path();
            SkinnedMesh* created_mesh = MeshFactory::createSkinnedMesh( skinned_mesh_path, joint_list );
            
            // Test if the mesh has a material
            fs::path material_path = skinned_mesh_dir.path();
            material_path.append( OBJECT_MATERIAL_FNAME );
            if(fs::exists( material_path )){
                // Use the found material
                std::string material_name = getPropertyContents( material_path );
                created_mesh->setMaterial( loaded_materials[material_name] );
            }
            // else { 
            //     // Use the default material
            //     created_mesh->setMaterial( loaded_materials["__DefaultMaterial"] );
            // }
            std::cout << "Created skinned mesh: " << skinned_mesh_path.filename().string() << " pointer: " << created_mesh << std::endl;
            loaded_skinned_meshes[skinned_mesh_path.filename().string()] = created_mesh;
        }
    }
}



void LevelLoader::loadCollisionShapes( fs::path dir ) {
    if( fs::exists(dir) ) {
        // Loop through all folders in the mesh dir and added them to the loaded_meshes
        for( auto& col_shape_dir: fs::directory_iterator(dir) ) {
            if(fswrapper::is_dir(col_shape_dir)) {
                fs::path col_shape_path = col_shape_dir.path();
                loaded_collision_shapes[col_shape_path.filename().string()] = RigidBodyFactory::createBvhTriangleMeshFromFiles( col_shape_path );
            }
        }
    }
}

void LevelLoader::loadMaterials( fs::path dir, fs::path textures_dir ) {
    if( fs::exists(dir) ) {
        for( auto& materials_dir: fs::directory_iterator( dir ) ) {
            if(fswrapper::is_dir(materials_dir)) {
                fs::path materials_path = materials_dir.path();
                loaded_materials[materials_path.filename().string()] = MaterialFactory::createMaterial( materials_path, textures_dir );
            }
        }
    }
}

void LevelLoader::loadAnimations(fs::path dir) {
    if( fs::exists(dir) ) {
        // Loop through all folders in the Animationstacks dir and add them to animation_stacks
        for( auto& animation_dir: fs::directory_iterator(dir) ) {
            if(fswrapper::is_dir(animation_dir)) {
                fs::path animation_path = animation_dir.path();
                animation_stacks.push_back(AnimationFactory::getAnimationStack(animation_path,scene));
            }
        }
    }
}

void LevelLoader::loadEndGame(fs::path dir){
    dir.append(LEVEL_ENDGAME_FNAME);
    if( fs::exists(dir) ){
        Asset endGameCoord(dir);
        endGameCoordVec = *(glm::vec3*)endGameCoord.getBuffer();
    }
}


void LevelLoader::loadJointList(fs::path dir) {
    if( fs::exists(dir) ) {
        fs::path joints_size_dir = dir;
        joints_size_dir.append("JointListSize");


        Asset size(joints_size_dir);
        int num_joints = *((int *)size.getBuffer());

        std::cout << "Loading joints: " << num_joints << std::endl;
        
        if(num_joints > 0) {

            joint_list = new JointList();

            for(int i = 0; i < num_joints; i++) {
                fs::path index_dir = dir;
                index_dir.append(std::to_string(i));
                joint_list->addJoint(loadJoint(index_dir));
            }
        }
    }
}



Joint LevelLoader::loadJoint(fs::path dir) {
    Joint j;

    fs::path joint_name_dir = dir;
    joint_name_dir.append("name");
    j.name = getPropertyContents(joint_name_dir);

    fs::path parent_dir = dir;
    parent_dir.append("parent");
    Asset parent_index_asset(parent_dir);
    j.parent_index = *((int *)parent_index_asset.getBuffer());

    return j;
}

// Create the scene GameObject and all of its children
void LevelLoader::createScene( LoadedObjectProperties *scene_root_props ) {
    scene = createGameObject( scene_root_props, true );
}

// Determine what GameObject should be created based on the Properties and create it
GameObject * LevelLoader::createGameObject( LoadedObjectProperties *obj_props, bool is_root ) {
    GameObject *obj;

    // Controls for determining types of game objects
    bool has_mesh = obj_props->mesh_id != std::string("");
    bool has_skinned_mesh = obj_props->skinned_mesh_id != std::string("");
    bool has_collision_shape = obj_props->collision_shape_id != std::string("");
    //bool has_material = obj_props->material_id != std::string("");
    bool is_bone = obj_props->is_bone;
    
    // Determine type of GameObject depending on the properties it has
    if( is_root ) {
        // std::cout << "Creating scene root: " << obj_props->identifier << std::endl;
        obj = new GameObject( obj_props->identifier );
    }
    else if( has_mesh && has_collision_shape ) {
        // std::cout << "Creating obstacle: " << obj_props->identifier << std::endl;
        Mesh *use_mesh = loaded_meshes[obj_props->mesh_id];
        btBvhTriangleMeshShape * use_shape = loaded_collision_shapes[obj_props->collision_shape_id];
        RigidBodyPhysicsComponent *use_physics = RigidBodyFactory::createBvhTriangleMeshComponent( obj_props->identifier, use_shape, obj_props->scaling );
        obj = new Obstacle( obj_props->identifier, use_mesh, use_physics );

        // Add a reference from the collision object to the GameObject it represents
        use_physics->getCollisionObject()->setUserPointer( (void *)obj );
    }
    else if( has_mesh && !has_collision_shape ) {
        // std::cout << "Creating a scene renderable: " << obj_props->identifier << std::endl;
        Mesh *use_mesh = loaded_meshes[obj_props->mesh_id];
        obj = new SceneRenderable( obj_props->identifier, use_mesh );
    }
    else if(has_skinned_mesh) {
        std::cout << "Creating a Player: " << obj_props->identifier << std::endl;
        SkinnedMesh* use_skinned_mesh = loaded_skinned_meshes[obj_props->skinned_mesh_id];
        obj = new Player(obj_props->identifier, use_skinned_mesh);
    }
    else if(is_bone) {
        std::cout << "Creating a bone: " << obj_props->identifier << std::endl;
        obj = new Bone(obj_props->identifier);
    }
    else {
        std::cout << "Creating Generic GameObject Node: " << obj_props->identifier << std::endl;
        obj = new GameObject(obj_props->identifier);
        // std::cerr << "Unknown object config: mesh - " << has_mesh << " | col - " << has_collision_shape << " | material - " << has_material << std::endl;
        // return nullptr;      std::cout << "Creating Generic GameObject"
    }

    // Set the transformation data for the newly created object
    // std::cout << "Scaling by " << obj_props->scaling.x << ", " << obj_props->scaling.y << ", " << obj_props->scaling.z << std::endl;
    // std::cout << "Rotating by " << obj_props->rotation.x << ", " << obj_props->rotation.y << ", " << obj_props->rotation.z << std::endl;
    // std::cout << "Translating by " << obj_props->translation.x << ", " << obj_props->translation.y << ", " << obj_props->translation.z << std::endl;
    glm::vec3 rotation_radians = glm::vec3(glm::radians(obj_props->rotation[0]), 
                                           glm::radians(obj_props->rotation[1]),
                                           glm::radians(obj_props->rotation[2]));
    glm::quat rotation_quaternion(rotation_radians);
    obj->setTransform( obj_props->scaling, rotation_quaternion, obj_props->translation );

    // Recurse through the created object's children, creating and adding them
    for( int i=0; i<obj_props->children.size(); i++ ) {
        GameObject *child_obj = createGameObject( obj_props->children[i], false );
        obj->addChild( child_obj );
    }

    return obj;
}
