#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <iostream>
#include <vector>

#include "GameObject.hpp"
#include "GameObjects/Obstacle.hpp"
#include "GameObjects/SceneRenderable.hpp"
#include "GameObjects/Player.hpp"

#include "Systems/Rendering/MeshFactory.hpp"
#include "Systems/Rendering/Material.hpp"
#include "Systems/Physics/RigidBodyFactory.hpp"
#include "Systems/Animation/AnimationFactory.hpp"
#include "Systems/Animation/Joint.hpp"

#include "Utilities/Asset.hpp"
#include "Utilities/WAIWrapper.hpp"
#include "Utilities/FilesystemWrapper.hpp"

// Root directory for any levels
#define LEVELS_DIRECTORY "Assets/Levels/"
#define PLAYER_MODEL_DIRECTORY "Assets/Player_Model"

// Directories of stored properties under the level root directory 
#define LEVEL_MESH_DNAME                "Meshes"
#define LEVEL_SKINNED_MESH_DNAME        "SkinnedMeshes"
#define LEVEL_MATERIALS_DNAME           "Materials"
#define LEVEL_COLLISION_SHAPES_DNAME    "Hitboxes"
#define LEVEL_TEXTURES_DNAME            "Textures"
#define LEVEL_ROOTNODE_DNAME            "RootNode"
#define LEVEL_ANIMATION_DNAME           "AnimationStacks"
#define LEVEL_ENDGAME_DNAME             "Endgame"

// Object subdirectory names for children or properties
#define OBJECT_CHILDREN_DNAME           "children"

// Filenames of property files in object hierarchy
#define OBJECT_MESH_FNAME               "Mesh.txt"
#define OBJECT_SKINNED_MESH_FNAME       "SkinnedMesh.txt"
#define OBJECT_MATERIAL_FNAME           "Material.txt"
#define OBJECT_COLLISION_SHAPE_FNAME    "Hitbox.txt"
#define OBJECT_ROTATION_FNAME           "rotation"
#define OBJECT_SCALING_FNAME            "scaling"
#define OBJECT_TRANSLATION_FNAME        "translation"
#define OBJECT_BONE_FNAME               "bone"
#define LEVEL_ENDGAME_FNAME             "endgame"

// Struct to store all the necessary properties to construct a GameObject from the level hierarchy
struct LoadedObjectProperties {
    std::string identifier = "";

    // Correspond to filenames under the approriate directory for the level
    // If left empty, the object does not have that property
    std::string mesh_id = "";
    std::string skinned_mesh_id = "";
    std::string collision_shape_id = "";

    // corresponds to a bone file in the directory. the value of the file does not matter,
    // just its existence
    bool is_bone = false;

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

    std::vector<AnimationStack*> animation_stacks;
    JointList * joint_list;

    // The objects loaded for the scene
    std::map<std::string, Mesh*> loaded_meshes;
    std::map<std::string, SkinnedMesh*> loaded_skinned_meshes;
    std::map<std::string, btBvhTriangleMeshShape*> loaded_collision_shapes;
    std::map<std::string, Material*> loaded_materials;
    //End Game Coordinates
    glm::vec3 endGameCoordVec;

    // Convenience functions
    fs::path levelPath( std::string level_name ) const;
    fs::path pathAppend( fs::path in, std::string to_append ) const;
    std::string getPropertyContents( fs::path property_path ) const;

    // Load a level from the name and populate the scene object
    void createLevel( std::string level_name );

    // Load information for a single object, then call recursively on object's children
    LoadedObjectProperties * parseObjectDirectory( std::string object_name, fs::path object_path ) const;
    
    // Create meshes, collision objects, materirals, and animations for the level from their directories
    void loadMeshes( fs::path dir );
    void loadSkinnedMeshes( fs::path dir );
    void loadCollisionShapes( fs::path dir );

    void loadMaterials( fs::path dir, fs::path textures_dir );
    void loadAnimations( fs::path dir );
    void loadEndGame( fs::path dir );
    void loadJointList( fs::path dir );
    Joint loadJoint( fs::path dir );

    // Create the scene GameObject and all of its children
    void createScene( LoadedObjectProperties * scene_root_props );

    // Determine what GameObject should be created based on the Properties and create it
    GameObject * createGameObject( LoadedObjectProperties * obj_props, bool is_root );

public:

    // Calls loadLevel with the passed in level name
    LevelLoader( std::string level_name );

    // Delete any stored info
    ~LevelLoader();

    // Return the loaded scene
    GameObject * getScene() { return scene; }

    // inline getters for animations
    int getNumAnimationStacks() { return (int)animation_stacks.size(); };
    AnimationStack* getAnimationStack(int i) { return animation_stacks[i]; };

    // return the joint list, null if no joints in this file
    JointList * getJointList() {return joint_list;}

    //return end game coordinates
    glm::vec3 getEndGameCoords(){ return endGameCoordVec; }


    static LevelLoader * loadCharacterModel();
    static LevelLoader * loadLevelFile(std::string name);
}; 

#endif