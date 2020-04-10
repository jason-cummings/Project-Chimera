#include "AnimationCurve.hpp"


void TranslationAnimationCurve::evaluate( float time ) {
	int t_key1 = -1;
	int t_key2 = -1;

	if(time > max_time) {
		int index = keyframes.size() - 1;

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

			float time_diff = key_2->time - key_1->time;
			float ratio = (time - key_1->time) / time_diff;

			glm::vec3 current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
			target->setTranslation(current_value);
		}
	}

	std::cout << "Selected keys: (" << t_key1 << ", " << t_key2 << ")" << std::endl;
}

void TranslationAnimationCurve::addKeyframe(VecKeyframe keyframe) {
	max_time = keyframe.time;
	keyframes.push_back(keyframe);
}



void RotationAnimationCurve::evaluate( float time ) {
	if(time > max_time) {
		int index = keyframes.size() - 1;

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

			float time_diff = key_2->time - key_1->time;
			float ratio = (time - key_1->time) / time_diff;

			glm::quat current_value = (1.0f - ratio) * key_1->value + ratio * key_2->value;
			target->setRotation(current_value);
		}
	}
}

void RotationAnimationCurve::addKeyframe(QuatKeyframe keyframe) {
	max_time = keyframe.time;
	keyframes.push_back(keyframe);
}