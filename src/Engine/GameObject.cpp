#include "GameObject.hpp"


GameObject::GameObject(std::string id) {
	identifier = id;
	transform = glm::mat4(1.0);
	world_transform = glm::mat4(1.0);
	parent = NULL;
}

GameObject::~GameObject() {
	// for(int i = 0; i < children.size(); i++) {
	// 	delete children[i];
	// }
}


// Transformation management

void GameObject::setTransform( glm::vec3 scale_in, glm::quat rot_in, glm::vec3 trans_in ) {
	scale = scale_in;
	rotation = rot_in;
	translation = trans_in;
	calculateTransforms();
}

void GameObject::setScale( glm::vec3 in ) {
	scale = in;
	calculateTransforms();
}

void GameObject::setRotation( glm::quat in ) {
	rotation = in;
	calculateTransforms();
}

void GameObject::setTranslation( glm::vec3 in ) {
	translation = in;
	calculateTransforms();
}

// Create transformation matrix from individual vectors
void GameObject::calculateTransforms() {
	glm::mat4 scale_matrix = glm::scale( glm::mat4(1.f), scale );
	glm::mat4 rotation_matrix = glm::toMat4( rotation );
	glm::mat4 translation_matrix = glm::translate( glm::mat4(1.f), translation );
	transform = translation_matrix * rotation_matrix * scale_matrix;

	if(parent != NULL)
		compileTransforms(parent->getWorldTransform());
	else
		compileTransforms(glm::mat4(1.f));
}

void GameObject::compileTransforms(glm::mat4 parent_transform) {
	world_transform = parent_transform * transform;
	for(int i = 0; i < children.size(); i++) {
		children[i]->compileTransforms(world_transform);
	}
}

// Gets worldspace transform from physics engine if applicable and converts to local space
void GameObject::updateTransformFromPhysics(glm::mat4 parent_transform) {
	// Nothing to be done by default, step through children
	for( int i=0; i<children.size(); i++ ) {
		children[i]->updateTransformFromPhysics(world_transform);
	}
}

// Update relevant bullet transforms as necessary (to be done just before physics update)
void GameObject::setBulletTransforms() const {
	// Nothing to be done by default, step through children
	for( int i=0; i<children.size(); i++ ) {
		children[i]->setBulletTransforms();
	}
}



// Scene graph management

GameObject * GameObject::getParent() {
	return parent;
}

void GameObject::setParent(GameObject * p) {
	parent = p;
}

int GameObject::getNumChildren() {
	return children.size();
}

GameObject * GameObject::getChild(int i) {
	if(i < children.size()) {
		return children[i];
	}
	return NULL;
}

void GameObject::addChild(GameObject * child) {
	children.push_back(child);
	child->setParent(this);
	child->compileTransforms(world_transform);
}

bool GameObject::removeChild(GameObject * child) {
	for(int i = 0; i < children.size(); i++) {
		if(children[i] == child) {
			children[i]->setParent(NULL);
			children.erase(children.begin() + i);
			return true;
		}
	}

	return false;
}

bool GameObject::removeChild(int child_index) {
	if(child_index < children.size()) {
		children[child_index]->setParent(NULL);
		children.erase(children.begin() + child_index);
		return true;
	}
	return false;
}

bool GameObject::deleteChild(GameObject * child){
	for(int i = 0; i < children.size(); i++) {
		if(children[i] == child) {
			children.erase(children.begin() + i);
			delete child;
			return true;
		}
	}

	return false;
}

bool GameObject::deleteChild(int child_index){
	if(child_index < children.size()) {
		delete children[child_index];
		children.erase(children.begin() + child_index);
		return true;
	}
	return false;
}



// Scene graph searching


GameObject * GameObject::getGameObject(std::string id) {
	if(id == identifier) {
		return this;
	}
	else {
		for(int i = 0; i < children.size(); i ++) {
			GameObject * result;
			if((result = children[i]->getGameObject(id)) != NULL) {
				return result;
			}
		}
	}

	return NULL;
}


// tests

bool testSetTransform() {
	GameObject * g1 = new GameObject("1");
	GameObject * g2 = new GameObject("2");
	g1->addChild(g2);
	g1->setTranslation( g1->getTranslation() + glm::vec3(1.0,0.0,0.0) );
	g2->setTranslation( glm::vec3(1.0,0.0,0.0) );

	glm::mat4 total = glm::translate(glm::mat4(1.0),glm::vec3(2.0,0.0,0.0));

	// std::cout << "total: " << glm::to_string(total) << std::endl;
	// std::cout << "g2: " << glm::to_string(g2->getWorldTransform()) << std::endl;
	bool equal = total == g2->getWorldTransform();
	delete g1; 
	delete g2;
	return equal;
}


bool testSearch() {
	GameObject g1 = GameObject("1");
	GameObject g2 = GameObject("2");
	g1.addChild(&g2);
	return g1.getGameObject("2") == &g2;
}

bool testRemove() {
	GameObject g1 = GameObject("1");
	GameObject g2 = GameObject("2");
	GameObject g3 = GameObject("3");
	g1.addChild(&g2);
	g1.addChild(&g3);
	g1.removeChild(&g2);
	g1.removeChild(0);
	return g1.getNumChildren() == 0;
}

// int main() {
// 	std::cout << "Transform tests: " << (testSetTransform()?"pass":"fail") << std::endl;
// 	std::cout << "Search tests: " << (testSearch()?"pass":"fail") << std::endl;
// 	std::cout << "Remove tests: " << (testRemove()?"pass":"fail") << std::endl;
// 	return 0;
// }
