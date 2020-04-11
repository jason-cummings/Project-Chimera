#ifndef ANIMATIONSTACK_HPP
#define ANIMATIONSTACK_HPP

#include "AnimationLayer.hpp"
#include <string>
#include <vector>
#include <map>
#include <utility>

class AnimationStack {
	std::string name;

	std::vector<AnimationLayer*> animations;

	// keeps track of all active animations
	std::vector<bool> active_animations;

	// stores indexes for each animation name for quick access
	std::map<std::string, int> animation_indexes;

public:
	AnimationStack(std::string stack_name);
	~AnimationStack();

	std::string getName() {return name;}

	void addAnimationLayer(AnimationLayer * layer);

	void evaluateAnimations(double dt);


	void startAnimation(std::string animation_name);
	void pauseAnimation(std::string animation_name);
	void unpauseAnimation(std::string animation_name);

	bool isActive(std::string animation_name);

	//blend animation


};




#endif