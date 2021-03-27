#ifndef ANIMATIONPROCESSOR_HPP
#define ANIMATIONPROCESSOR_HPP

#include <fbxsdk.h>
#include <vector>

#include "../DataOptimizer.hpp"

struct Keyframe {
    double time;
    float value[3];
};

class AnimationProcessor {
private:
    DataOptimizer animationOptimizer;

    std::vector<fbxsdk::FbxAnimLayer *> animations;
    std::vector<std::string> animationDirectories;

    AnimationProcessor(std::string baseDir);

public:
    static AnimationProcessor &getInstance();

    // processes animation data
    void processAnimationStack(fbxsdk::FbxAnimStack *animationStack);
    void processNodeForAnimation(fbxsdk::FbxNode *node);
    void saveKeyframes(fbxsdk::FbxNode *node, fbxsdk::FbxAnimCurve *xCurve, fbxsdk::FbxAnimCurve *yCurve, fbxsdk::FbxAnimCurve *zCurve, int animationIndex, std::string filename);
};

#endif