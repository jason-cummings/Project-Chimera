#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include "AnimationStack.hpp"
#include <vector>
#include <map>
#include <utility>

class AnimationSystem {
	std::vector<AnimationStack*> stacks;
	std::map<std::string, int> stack_indexes;

public:
	~AnimationSystem();
	AnimationStack * getAnimationStack(std::string stack_name);
	void addAnimationStack(AnimationStack* stack);

	void evaluateAnimations(double dt);
};




#endif