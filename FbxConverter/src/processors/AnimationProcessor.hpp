#ifndef ANIMATIONPROCESSOR_HPP
#define ANIMATIONPROCESSOR_HPP

#include <fbxsdk.h>

#include "../DataOptimizer.hpp"

struct Keyframe {
    double time;
    float value[3];
};

class AnimationProcessor {
private:
    DataOptimizer animation_optimizer;

    std::vector<fbxsdk::FbxAnimLayer *> animations;
    std::vector<std::string> animation_directories;

    AnimationProcessor(std::string base_dir);

public:
    static AnimationProcessor &getInstance();

    // processes animation data
    void processAnimationStack(fbxsdk::FbxAnimStack *animation_stack);
    void processNodeForAnimation(fbxsdk::FbxNode *node);
    void saveKeyframes(fbxsdk::FbxNode *node, fbxsdk::FbxAnimCurve *x_curve, fbxsdk::FbxAnimCurve *y_curve, fbxsdk::FbxAnimCurve *z_curve, int animation_index, std::string filename);
};

#endif