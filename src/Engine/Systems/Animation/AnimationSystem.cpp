#include "AnimationSystem.hpp"


AnimationSystem::~AnimationSystem() {
	for(int i = 0; i < stacks.size(); i++) {
		delete stacks[i];
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

void AnimationSystem::evaluateAnimations(double dt) {
	for(int i = 0; i < stacks.size(); i++) {
		stacks[i]->evaluateAnimations(dt);
	}
}