#ifndef JOINT_H
#define JOINT_H
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include "./../../GameObjects/Bone.hpp"

struct Joint {
	int parent_index;
	std::string name;
	Bone * bone_pointer;
};

// jointlist objects maintain a list of all bones from an imported level
// the use of this object is to create the list of transformation matrices for skeletal animation
// this object will be contained by animation system for purposes of recalculating 
// the transformation matrices and for deallocating memory
// Every instance of skinned mesh will have a pointer to a JointList object for rendering purposes
class JointList {
private:
	std::vector<Joint> joint_list;
	std::vector<glm::mat4> transform_matrices;

public:


	void addJoint(Joint j);

	int getNumBones();

	std::vector<glm::mat4> * getTransformMatrices();

	// sets the pointers for each Joint object to a bone gameobject
	//    - this is needed due to the order of initializing objects
	void setJointPointers(GameObject * scene);

	// calls the calculateInverseBindPose function for every bone. This should only be
	// called once at initialization
	void calculateInverseBindPose();

	// recalculates the transformation matrices of all bones
	void updateTransformationMatrices();

};


#endif