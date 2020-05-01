#ifndef ANIMATIONSTACK_HPP
#define ANIMATIONSTACK_HPP

#include "AnimationLayer.hpp"
#include <string>
#include <vector>
#include <map>
#include <utility>

struct AnimationSequenceInfo {

	bool started;

	bool blend_mode;
	float blend_duration;
	float blend_cur_time;
	
	bool anim1_start_from_beginning;
	bool anim1_loop;
	bool anim1_complete;
	std::string animation1;
	int anim1_index;

	bool anim2_start_from_beginning;
	bool anim2_loop;
	bool anim2_complete;
	std::string animation2;
	int anim2_index;

	AnimationSequenceInfo() {
		started = false;
		blend_mode = false;
		blend_duration = 0.0;
		blend_cur_time = 0.0;

		anim1_start_from_beginning = true;
		anim1_loop = false;
		anim1_complete = false;

		anim2_start_from_beginning = true;
		anim2_loop = false;
		anim2_complete = false;

	}
};

class AnimationStack {
	std::string name;

	std::vector<AnimationLayer*> animations;

	// keeps track of all active animations
	std::vector<bool> active_animations;

	// stores indexes for each animation name for quick access
	std::map<std::string, int> animation_indexes;

	bool sequence_active;
	AnimationSequenceInfo sequence;

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

	AnimationSequenceInfo getAnimationSequence() { return sequence; }
	void setAnimationSequence(AnimationSequenceInfo sequence_in);

	//blend animation


};




#endif