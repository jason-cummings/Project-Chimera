#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Systems/Rendering/Mesh.hpp"
#include "Systems/Physics/PhysicsComponent.hpp"


class GameObject {
protected:
	int identifier; // use to reconstruct scenegraph from file
	std::vector<GameObject *> children;
	GameObject * parent; // this can be null
	glm::mat4 transform; // transform from parent space
	glm::mat4 world_transform; // transform from world space

public:
	//constructor
	GameObject(int id);
	virtual ~GameObject();

	virtual bool hasMesh() const { return false; }
	virtual Mesh * getMesh() const { return (Mesh *) nullptr; }
	virtual bool hasPhysicsComponent() const { return false; }
	virtual PhysicsComponent * getPhysicsComponent() const { return (PhysicsComponent *) nullptr; }

	// Update function
	// This should only ever be used for updating or retrieving data
	// GameObjects should not be controlling or modifying themselves otherwise
	// virtual void update();

	//transformation management
	glm::mat4 getTransform() const { return transform; }
	glm::mat4 getWorldTransform() const { return world_transform; }

	void setTransform(glm::mat4 in);
	virtual void compileTransforms(glm::mat4 parent_transform); //compiles tranformation matrices into world_transform
	virtual void updateTransformFromPhysics(glm::mat4 parent_transform); // Gets worldspace transform from physics engine and converts to local space
	virtual void setBulletTransforms() const;

	int getID() const { return identifier; }

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

	virtual GameObject * getGameObject(int id); // used for searching scene graph for a given identifier

};




#endif