#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include "AnimationStack.hpp"
#include "Joint.hpp"
#include <map>
#include <utility>
#include <vector>

class AnimationSystem {
    std::vector<AnimationStack *> stacks;
    std::map<std::string, int> stack_indexes;

    std::vector<JointList *> joints;

  public:
    ~AnimationSystem();
    AnimationStack *getAnimationStack(std::string stack_name);
    void addAnimationStack(AnimationStack *stack);

    void addJointList(JointList *joint_list);

    void evaluateAnimations(double dt);
};

#endif