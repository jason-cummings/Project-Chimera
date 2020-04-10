#include "AnimationStack.hpp"

AnimationStack::AnimationStack(std::string stack_name) {
	name = stack_name;
}
AnimationStack::~AnimationStack() {
	for(int i = 0; i < animations.size(); i++) {
		delete animations[i];
	}
}

void AnimationStack::addAnimationLayer(AnimationLayer * layer) {
	int index = animations.size();
	animations.push_back(layer);
	animation_indexes.insert ( std::pair<std::string,int>(layer->getName(),index) );
	active_animations.push_back(true);
}

void AnimationStack::evaluateAnimations(double dt) {
	for(int i = 0; i < animations.size(); i++) {
		if(active_animations[i]) {
			std::cout << "Evaluating animations for " << animations[i]->getName() <<std::endl;
			animations[i]->evaluate(dt);
		}
	}
}


void AnimationStack::startAnimation(std::string animation_name) {
	int index = animation_indexes[animation_name];
	animations[index]->resetTime();
	active_animations[index] = true;
}
void AnimationStack::pauseAnimation(std::string animation_name) {
	int index = animation_indexes[animation_name];
	active_animations[index] = false;
}
void AnimationStack::unpauseAnimation(std::string animation_name) {
	int index = animation_indexes[animation_name];
	active_animations[index] = true;
}

bool AnimationStack::isActive(std::string animation_name) {
	int index = animation_indexes[animation_name];
	return active_animations[index];
}