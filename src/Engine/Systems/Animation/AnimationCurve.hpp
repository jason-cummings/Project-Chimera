#ifndef ANIMATIONCURVE_HPP
#define ANIMATIONCURVE_HPP

#include <vector>

#include "Keyframe.hpp"
#include "../../GameObject.hpp"

class AnimationCurve {
protected:
	GameObject * target;
	float max_time = 0;

public:
	virtual ~AnimationCurve(){}


	void setTarget(GameObject * t) {target = t;}
	GameObject * getTarget() {return target;}
	float getMaxTime() {return max_time;}


	virtual void evaluate(float time) = 0;

	virtual void evaluateWithBlend(float time, float blend_amount) = 0;

	// use friend for factory?

};

class TranslationAnimationCurve : public AnimationCurve {
	std::vector<VecKeyframe> keyframes;
public:
	void evaluate(float time);
	void evaluateWithBlend(float time, float blend_amount);
	void addKeyframe(VecKeyframe keyframe);
};

class RotationAnimationCurve : public AnimationCurve {
	std::vector<QuatKeyframe> keyframes;
public:
	void evaluate(float time);
	void evaluateWithBlend(float time, float blend_amount);
	void addKeyframe(QuatKeyframe keyframe);
};





#endif