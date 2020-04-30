#include "Joint.hpp"

void JointList::addJoint(Joint j) {
	joint_list.push_back(j);
	transform_matrices.push_back(glm::mat4(1.0));
}

int JointList::getNumBones() {
	return joint_list.size();
}

std::vector<glm::mat4> * JointList::getTransformMatrices() {
	return &(transform_matrices);
}

void JointList::setJointPointers(GameObject * scene) {
	for(int i = 0; i < joint_list.size(); i++) {
		joint_list[i].bone_pointer = (Bone *) scene->getGameObject(joint_list[i].name);
	}
}

void JointList::calculateInverseBindPose() {
	for(int i = 0; i < joint_list.size(); i++) {
		joint_list[i].bone_pointer->calculateInverseBindPose();
	}
}

void JointList::updateTransformationMatrices() {
	for(int i = 0; i < joint_list.size(); i++) {
		transform_matrices[i] = joint_list[i].bone_pointer->getAnimatedTransformMatrix();
	}
}