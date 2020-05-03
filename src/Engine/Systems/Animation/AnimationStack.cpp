#include "AnimationStack.hpp"

AnimationStack::AnimationStack(std::string stack_name) {
	name = stack_name;
	sequence_active = false;
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
	active_animations.push_back(false);
}

void AnimationStack::evaluateAnimations(double dt) {
	for(int i = 0; i < animations.size(); i++) {
		if(active_animations[i]) {
			animations[i]->evaluate(dt);
		}
	}

	if(sequence_active) {
		if(sequence.anim1_complete) {
			if(sequence.anim2_complete) {
				sequence_active = false;
			}
			else {
				animations[sequence.anim2_index]->evaluate(dt);
				if(!sequence.anim2_loop && animations[sequence.anim2_index]->getComplete()) {
					sequence.anim2_complete = true;
				}
			}
		}
		else {
			if(sequence.blend_mode) {
				sequence.blend_cur_time += dt;
				if(sequence.blend_cur_time > sequence.blend_duration) {
					sequence.anim1_complete = true;
				}
				else {
					animations[sequence.anim1_index]->evaluate(dt);
					animations[sequence.anim2_index]->evaluateWithBlend(dt, sequence.blend_cur_time / sequence.blend_duration);
				}

			}
			else {
				animations[sequence.anim1_index]->evaluate(dt);
				if(!sequence.anim1_loop && animations[sequence.anim1_index]->getComplete()) {
					sequence.anim1_complete = true;
				}
			}
		}
	}
}

void AnimationStack::startAllAnimations() {
	for(int i = 0; i < active_animations.size(); i++) {
		active_animations[i] = true;
	}
}

void AnimationStack::stopAllAnimations() {
	for(int i = 0; i < active_animations.size(); i++) {
		active_animations[i] = false;
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

void AnimationStack::setAnimationSequence(AnimationSequenceInfo sequence_in) { 
	sequence = sequence_in; 
	sequence_active = true;

	sequence.anim1_index = animation_indexes[sequence.animation1];
	sequence.anim2_index = animation_indexes[sequence.animation2];

	if(sequence.anim1_start_from_beginning) {
		animations[sequence.anim1_index]->resetTime();
	}

	if(sequence.anim2_start_from_beginning) {
		animations[sequence.anim2_index]->resetTime();
	}
}

