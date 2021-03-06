#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

class GameObject;

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Systems/Rendering/Renderable.hpp"
// #include "Systems/Rendering/SkinnedMesh.hpp"
// #include "Systems/Rendering/OverlayMesh.hpp"
// #include "Systems/Rendering/Renderable.hpp"
#include "Systems/Physics/RigidBodyPhysicsComponent.hpp"

class Mesh;
class SkinnedMesh;
class OverlayMesh;

class GameObject {
  protected:
    std::string identifier;     // use to reconstruct scenegraph from file
    bool destroy_all_on_delete; // Control whether or not to destroy all children when deleted
    std::vector<GameObject *> children;
    GameObject *parent; // this can be null

    // Scaling, rotation and translation for the object
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::mat4 transform;              // transform from parent space
    glm::mat4 world_transform;        // transform from world space
    glm::mat4 bullet_world_transform; // transform with no scale component

    // Method to calculate resulting transformation matrix
    void calculateTransforms();

  public:
    //constructor
    GameObject(std::string id);
    virtual ~GameObject();

    virtual void print();
    virtual void print(std::string depth);

    std::string getID() const { return identifier; }

    virtual bool hasRenderable() const { return false; }
    virtual Renderable *getRenderable() const { return (Renderable *)nullptr; }

    virtual bool hasPhysicsComponent() const { return false; }
    virtual RigidBodyPhysicsComponent *getPhysicsComponent() const { return nullptr; }

    //transformation management
    inline glm::mat4 getTransform() const { return transform; }
    inline glm::vec3 getScale() const { return scale; }
    inline glm::quat getRotation() const { return rotation; }
    inline glm::vec3 getTranslation() const { return translation; }
    inline glm::mat4 getWorldTransform() const { return world_transform; }
    inline glm::mat4 getBulletWorldTransform() const { return bullet_world_transform; }

    void setTransform(glm::vec3 scale_in, glm::quat rot_in, glm::vec3 trans_in);
    void setScale(glm::vec3 in);
    void setRotation(glm::quat in);
    void setTranslation(glm::vec3 in);

    virtual void compileTransforms(glm::mat4 parent_transform); //compiles tranformation matrices into world_transform

    // Physics related transform management
    virtual void updateTransformFromPhysics(glm::vec3 parent_scale, glm::mat4 parent_bullet_transform); // Gets worldspace transform from physics engine and converts to local space
    virtual void setBulletTransforms() const;                                                           // Update physics body transforms just before physics step

    // scenegraph / child object management
    GameObject *getParent();
    void setParent(GameObject *p);
    inline void setDestroyAll(bool new_val) { destroy_all_on_delete = new_val; }

    int getNumChildren();

    GameObject *getChild(int i);

    void addChild(GameObject *child); // adds the child to the list of children and sets the child's parent to this gameobject

    bool removeChild(GameObject *child); // removes the child from the list of children,
                                         // and changes the child's parent to null - does not delete the child
    bool removeChild(int child_index);

    bool deleteChild(GameObject *child); // removes the child from the list of children and deletes it
    bool deleteChild(int child_index);

    void deleteAllChildren(); // Recursively call

    // searching the scenegraph

    virtual GameObject *getGameObject(std::string id); // used for searching scene graph for a given identifier
};

#endif