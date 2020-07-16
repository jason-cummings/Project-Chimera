#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <map>
#include <vector>
#include <glm/vec3.hpp>

#include "Utilities/FilesystemWrapper.hpp"
#include "Systems/Animation/Joint.hpp"

class Mesh;
class SkinnedMesh;
class Material;
class AnimationStack;
class btBvhTriangleMeshShape;

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
    JointList * getJointList() { return joint_list; }

    static LevelLoader * loadCharacterModel();
    static LevelLoader * loadLevelFile(std::string name);
}; 

#endif