#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Systems/Rendering/Mesh.hpp"
#include "Systems/Physics/RigidBodyPhysicsComponent.hpp"


class GameObject {
protected:
	std::string identifier; // use to reconstruct scenegraph from file
	std::vector<GameObject *> children;
	GameObject * parent; // this can be null
	glm::mat4 transform; // transform from parent space
	glm::mat4 world_transform; // transform from world space

public:
	//constructor
	GameObject(std::string id);
	virtual ~GameObject();

	std::string getID() const { return identifier; }

	virtual bool hasMesh() const { return false; }
	virtual Mesh * getMesh() const { return (Mesh *) nullptr; }
	virtual bool hasPhysicsComponent() const { return false; }
	virtual RigidBodyPhysicsComponent * getPhysicsComponent() const { return nullptr; }

	//transformation management
	glm::mat4 getTransform() const { return transform; }
	glm::mat4 getWorldTransform() const { return world_transform; }

	void setTransform(glm::mat4 in);
	virtual void compileTransforms(glm::mat4 parent_transform); //compiles tranformation matrices into world_transform

	// Physics related transform management
	virtual void updateTransformFromPhysics(glm::mat4 parent_transform); // Gets worldspace transform from physics engine and converts to local space
	virtual void setBulletTransforms() const; // Update physics body transforms just before physics step

	// scenegraph / child object management
	GameObject * getParent();
	void setParent(GameObject * p);

	int getNumChildren();

	GameObject * getChild(int i);

	void addChild(GameObject * child); // adds the child to the list of children and sets the child's parent to this gameobject

	bool removeChild(GameObject * child); // removes the child from the list of children, 
		// and changes the child's parent to null - does not delete the child
	bool removeChild(int child_index);
	
	bool deleteChild(GameObject * child); // removes the child from the list of children and deletes it
	bool deleteChild(int child_index);

	// searching the scenegraph

	virtual GameObject * getGameObject(std::string id); // used for searching scene graph for a given identifier

};




#endif