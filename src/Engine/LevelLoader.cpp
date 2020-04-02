#include "LevelLoader.hpp"

// Calls loadLevel with the passed in level name
LevelLoader::LevelLoader( std::string level_name ) {
    scene = nullptr;
    loadLevel(level_name);
}

// Delete any stored info
LevelLoader::~LevelLoader() {

}

// Get the root path of the level
fs::path LevelLoader::levelPath( std::string level_name ) const {
    return (fs::path)(WAIWrapper::getExecutablePath() +  "/" + LEVELS_DIRECTORY + level_name);
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

void LevelLoader::loadLevel( std::string level_name ) {
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

    // Create the scene with the properties read in
    createScene( scene_properties );
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
    fs::path obj_material_path =        pathAppend( object_path, OBJECT_MATERIAL_FNAME );
    fs::path obj_collision_shape_path = pathAppend( object_path, OBJECT_COLLISION_SHAPE_FNAME );
    fs::path obj_rotation_path =        pathAppend( object_path, OBJECT_ROTATION_FNAME );
    fs::path obj_scaling_path =         pathAppend( object_path, OBJECT_SCALING_FNAME );
    fs::path obj_translation_path =     pathAppend( object_path, OBJECT_TRANSLATION_FNAME );
    fs::path obj_children_path =        pathAppend( object_path, OBJECT_CHILDREN_DNAME );

    // Read in the property indicators
    new_obj->mesh_id =                  getPropertyContents( obj_mesh_path );
    new_obj->material_id =              getPropertyContents( obj_material_path );
    new_obj->collision_shape_id =       getPropertyContents( obj_collision_shape_path );

    // Read in asset files for the transformation data
    Asset obj_rotation_asset( obj_rotation_path );
    Asset obj_scaling_asset( obj_scaling_path );
    Asset obj_translation_asset( obj_translation_path );

    // Copy the data to the appropriate object properties
    new_obj->rotation = (float *)obj_rotation_asset.copyBuffer();
    new_obj->scaling = (float *)obj_scaling_asset.copyBuffer();
    new_obj->translation = (float *)obj_translation_asset.copyBuffer();

    // Loop over the children and populate the current object's children
    if( fs::exists(obj_children_path) ) {
        for( auto& child_dir: fs::directory_iterator(obj_children_path) ) {
            if( is_directory(child_dir) ) {
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

// Create the scene GameObject and all of its children
void LevelLoader::createScene( LoadedObjectProperties *scene_root ) {

}

// Determine what GameObject should be created based on the Properties and create it
GameObject * LevelLoader::createGameObject( LoadedObjectProperties *obj ) {
    GameObject *temp = new GameObject("temp");
    return temp;
}
