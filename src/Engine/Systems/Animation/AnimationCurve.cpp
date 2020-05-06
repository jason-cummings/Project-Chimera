#include "AnimationCurve.hpp"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

void TranslationAnimationCurve::evaluate( float time ) {
	int t_key1 = -1;
	int t_key2 = -1;

	if(time > max_time) {
		int index = (int)keyframes.size() - 1;

		// set values
		t_key1 = index;

		target->setTranslation(keyframes[index].value);
	}
	else {
		int index1=-1;
		int i = 0;
		while(i < keyframes.size() && time > keyframes[i].time) {
			index1 = i;
			i++;
		}

		if(i == 0) {
			t_key2 = 0;
			target->setTranslation(keyframes[0].value);
		}
		else {
			VecKeyframe * key_1 = &(keyframes[index1]);
			VecKeyframe * key_2 = &(keyframes[i]);

			t_key1 = index1;
			t_key2 = i;

			float time_diff = (float)(key_2->time - key_1->time);
			float ratio = (float)((time - key_1->time) / time_diff);

			glm::vec3 current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
			target->setTranslation(current_value);
		}
	}
}

void TranslationAnimationCurve::evaluateWithBlend(float time, float blend_amount) {
	int t_key1 = -1;
	int t_key2 = -1;

	glm::vec3 current_value;

	if(time > max_time) {
		int index = (int)keyframes.size() - 1;

		// set values
		t_key1 = index;

		current_value = keyframes[index].value;
	}
	else {
		int index1=-1;
		int i = 0;
		while(i < keyframes.size() && time > keyframes[i].time) {
			index1 = i;
			i++;
		}

		if(i == 0) {
			t_key2 = 0;
			current_value = keyframes[0].value;
		}
		else {
			VecKeyframe * key_1 = &(keyframes[index1]);
			VecKeyframe * key_2 = &(keyframes[i]);

			t_key1 = index1;
			t_key2 = i;

			float time_diff = (float)(key_2->time - key_1->time);
			float ratio = (float)((time - key_1->time) / time_diff);

			current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
		}
	}

	glm::vec3 blended_vector = (1.0f - blend_amount) * target->getTranslation() + blend_amount * current_value;
	target->setTranslation(blended_vector);
}

void TranslationAnimationCurve::addKeyframe(VecKeyframe keyframe) {
	max_time = (float)keyframe.time;
	keyframes.push_back(keyframe);
}



void RotationAnimationCurve::evaluate( float time ) {
	if(time > max_time) {
		int index = (int)keyframes.size() - 1;

		// set values

		target->setRotation(keyframes[index].value);
	}
	else {
		int index1=-1;
		int i = 0;
		while(i < keyframes.size() && time > keyframes[i].time) {
			index1 = i;
			i++;
		}

		if(i == 0) {
			target->setRotation(keyframes[0].value);
		}
		else {
			QuatKeyframe * key_1 = &(keyframes[index1]);
			QuatKeyframe * key_2 = &(keyframes[i]);

			float time_diff = (float)(key_2->time - key_1->time);
			float ratio = (float)((time - key_1->time) / time_diff);

			glm::quat current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
			target->setRotation(current_value);
		}
	}
}

void RotationAnimationCurve::evaluateWithBlend(float time, float blend_amount) {
	int t_key1 = -1;
	int t_key2 = -1;

	glm::quat current_value;

	if(time > max_time) {
		int index = (int)keyframes.size() - 1;

		// set values
		t_key1 = index;

		current_value = keyframes[index].value;
	}
	else {
		int index1=-1;
		int i = 0;
		while(i < keyframes.size() && time > keyframes[i].time) {
			index1 = i;
			i++;
		}

		if(i == 0) {
			t_key2 = 0;
			current_value = keyframes[0].value;
		}
		else {
			QuatKeyframe * key_1 = &(keyframes[index1]);
			QuatKeyframe * key_2 = &(keyframes[i]);

			t_key1 = index1;
			t_key2 = i;

			float time_diff = (float)(key_2->time - key_1->time);
			float ratio = (float)((time - key_1->time) / time_diff);

			current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
		}
	}

	glm::quat blended_vector = (1.0f - blend_amount) * target->getRotation() + blend_amount * current_value;
	target->setRotation(blended_vector);
}

void RotationAnimationCurve::addKeyframe(QuatKeyframe keyframe) {
	max_time = (float)keyframe.time;
	keyframes.push_back(keyframe);
}