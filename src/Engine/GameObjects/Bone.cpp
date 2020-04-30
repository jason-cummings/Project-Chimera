#include "Bone.hpp"


glm::mat4 Bone::getAnimatedTransformMatrix() {
	return getWorldTransform() * inverseBindPose;
}

void Bone::calculateInverseBindPose() {
	inverseBindPose = glm::inverse(getWorldTransform());
}