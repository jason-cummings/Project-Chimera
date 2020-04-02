#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Asset.hpp"
#include "GameObject.hpp"
#include "WAIWrapper.hpp"

// Root directory for any levels
#define LEVELS_DIRECTORY "Assets/Levels/"

// Directories of stored properties under the level root directory 
#define LEVEL_MESH_DNAME                "Meshes"
#define LEVEL_MATERIALS_DNAME           "Materials"
#define LEVEL_COLLISION_SHAPES_DNAME    "Hitboxes"
#define LEVEL_ROOTNODE_DNAME            "RootNode"

// Object subdirectory names for children or properties
#define OBJECT_CHILDREN_DNAME           "children"

// Filenames of property files in object hierarchy
#define OBJECT_MESH_FNAME               "Mesh.txt"
#define OBJECT_MATERIAL_FNAME           "Material.txt"
#define OBJECT_COLLISION_SHAPE_FNAME    "Hitbox.txt"
#define OBJECT_ROTATION_FNAME           "rotation"
#define OBJECT_SCALING_FNAME            "scaling"
#define OBJECT_TRANSLATION_FNAME        "translation"

// Struct to store all the necessary properties to construct a GameObject from the level hierarchy
struct LoadedObjectProperties {
    std::string identifier = "";

    // Correspond to filenames under the approriate directory for the level
    std::string mesh_id = "";
    std::string collision_shape_id = "";
    std::string material_id = "";

    // Transformation values
    float *rotation = nullptr;
    float *scaling = nullptr;
    float *translation = nullptr;

    // Construction info for the object's children
    std::vector<LoadedObjectProperties *> children;

    ~LoadedObjectProperties() {
        if( rotation ) free(rotation);
        if( scaling ) free(scaling);
        if( translation ) free(translation);
    }
};

class LevelLoader {
private:
    // The root scene object to be created
    GameObject *scene;

    // Convenience functions
    fs::path levelPath( std::string level_name ) const;
    fs::path pathAppend( fs::path in, std::string to_append ) const;
    std::string getPropertyContents( fs::path property_path ) const;

    // Load a level from the name and populate the scene object
    void loadLevel( std::string level_name );

    // Load information for a single object, then call recursively on object's children
    LoadedObjectProperties * parseObjectDirectory( std::string object_name, fs::path object_path ) const;

    // Create the scene GameObject and all of its children
    void createScene( LoadedObjectProperties * scene_root );

    // Determine what GameObject should be created based on the Properties and create it
    GameObject * createGameObject( LoadedObjectProperties * obj );

public:
    // Calls loadLevel with the passed in level name
    LevelLoader( std::string level_name );

    // Delete any stored info
    ~LevelLoader();

    // Return the loaded scene
    GameObject * getScene() { return scene; }
}; 

#endif