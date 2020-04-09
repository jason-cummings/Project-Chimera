#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>
#include <vector>

#include "Asset.hpp"
#include "GameObject.hpp"
#include "GameObjects/Obstacle.hpp"
#include "GameObjects/SceneRenderable.hpp"

#include "Systems/Rendering/MeshFactory.hpp"
#include "Systems/Physics/RigidBodyFactory.hpp"

#include "WAIWrapper.hpp"
#include "FilesystemWrapper.hpp"

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
    // If left empty, the object does not have that property
    std::string mesh_id = "";
    std::string collision_shape_id = "";
    std::string material_id = "";

    // Transformation values
    glm::vec3 scaling = glm::vec3(1.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 translation = glm::vec3(0.f);

    // Construction info for the object's children
    std::vector<LoadedObjectProperties *> children;

    ~LoadedObjectProperties() {}
};

class LevelLoader {
private:
    // The root scene object to be created
    GameObject *scene;

    // The objects loaded for the scene
    std::map<std::string, Mesh*> loaded_meshes;
    std::map<std::string, btBvhTriangleMeshShape*> loaded_collision_shapes;

    // Convenience functions
    fs::path levelPath( std::string level_name ) const;
    fs::path pathAppend( fs::path in, std::string to_append ) const;
    std::string getPropertyContents( fs::path property_path ) const;

    // Load a level from the name and populate the scene object
    void createLevel( std::string level_name );

    // Load information for a single object, then call recursively on object's children
    LoadedObjectProperties * parseObjectDirectory( std::string object_name, fs::path object_path ) const;
    
    // Create meshes, collision objects, and materirals for the level from their directories
    void loadMeshes( fs::path dir );
    void loadCollisionShapes( fs::path dir );
    void loadMaterials( fs::path dir );

    // Create the scene GameObject and all of its children
    void createScene( LoadedObjectProperties * scene_root_props );

    // Determine what GameObject should be created based on the Properties and create it
    GameObject * createGameObject( LoadedObjectProperties * obj_props, bool is_root );
    
    // Calls loadLevel with the passed in level name
    LevelLoader( std::string level_name );
    
    // Return the loaded scene
    GameObject * getScene() { return scene; }

public:
    // Delete any stored info
    ~LevelLoader();

    static GameObject * loadLevel( std::string level_name );
}; 

#endif