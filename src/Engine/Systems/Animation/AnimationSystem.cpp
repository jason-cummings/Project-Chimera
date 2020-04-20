#include "AnimationSystem.hpp"


AnimationSystem::~AnimationSystem() {
	for(int i = 0; i < stacks.size(); i++) {
		delete stacks[i];
	}
	for(int i = 0; i < joints.size(); i++) {
		delete joints[i];
	}
}

AnimationStack * AnimationSystem::getAnimationStack(std::string stack_name) {
	return stacks[stack_indexes[stack_name]];
}

void AnimationSystem::addAnimationStack(AnimationStack* stack) {
	int index = stacks.size();
	stacks.push_back(stack);
	stack_indexes.insert ( std::pair<std::string,int>(stack->getName(),index) );
}

void AnimationSystem::addJointList(JointList * joint_list) {
	joints.push_back(joint_list);
}

void AnimationSystem::evaluateAnimations(double dt) {
	for(int i = 0; i < stacks.size(); i++) {
		stacks[i]->evaluateAnimations(dt);
	}

	for(int i = 0; i < joints.size(); i++) {
		joints[i]->updateTransformationMatrices();
	}
}