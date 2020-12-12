#ifndef BONE_HPP
#define BONE_HPP

class Bone;

#include "./../GameObject.hpp"

class Bone : public GameObject {
    glm::mat4 inverseBindPose;

  public:
    Bone(std::string id) : GameObject(id) {}

    glm::mat4 getAnimatedTransformMatrix();

    void calculateInverseBindPose();
};

#endif