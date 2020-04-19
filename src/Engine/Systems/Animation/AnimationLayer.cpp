#include "AnimationLayer.hpp"

AnimationLayer::AnimationLayer(std::string layer_name) {
	name = layer_name;
	cur_time = 0;
	max_time = .1;
}

AnimationLayer::~AnimationLayer() {
	for(int i = 0; i < curves.size(); i++) {
		delete curves[i];
	}
}

// updates the cur_time variable and evaluates the animation curves
void AnimationLayer::evaluate(double dt) {
	cur_time += dt;
	while(cur_time > max_time) {
		cur_time -= max_time;
	}

	for(int i = 0; i < curves.size(); i++) {
		curves[i]->evaluate(cur_time);
	}
}

// resets the animation layer's current time to 0, for restarting the animation from the beginning
void AnimationLayer::resetTime() {
	cur_time = 0;
}

void AnimationLayer::addCurve(AnimationCurve* new_curve) {
	curves.push_back(new_curve);
	if(new_curve->getMaxTime() > max_time)
		max_time = new_curve->getMaxTime();
}