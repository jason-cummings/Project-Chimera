#ifndef KEYFRAME_HPP
#define KEYFRAME_HPP

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

struct VecKeyframe {
    double time;
    glm::vec3 value;
};

struct QuatKeyframe {
    double time;
    glm::quat value;

    QuatKeyframe() {}
    QuatKeyframe(VecKeyframe v_keyframe) {
        time = v_keyframe.time;
        value = glm::quat(v_keyframe.value);
    }
};

#endif