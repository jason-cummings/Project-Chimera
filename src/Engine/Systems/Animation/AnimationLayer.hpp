#ifndef ANIMATIONLAYER_HPP
#define ANIMATIONLAYER_HPP

#include "AnimationCurve.hpp"
#include <string>
#include <vector>

class AnimationLayer {
	std::string name;
	std::vector<AnimationCurve*> curves;
	float cur_time;
	float max_time;

public:
	AnimationLayer(std::string layer_name);
	~AnimationLayer();

	void evaluate(double dt);
	void resetTime();

	void addCurve(AnimationCurve* new_curve);

	std::string getName() {
		return name;
	}
};





#endif