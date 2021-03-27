#include "AnimationProcessor.hpp"

#include <fstream>

#include "../Logger.hpp"

AnimationProcessor::AnimationProcessor(std::string baseDir) : animationOptimizer(baseDir) {
}

AnimationProcessor &AnimationProcessor::getInstance() {
    static AnimationProcessor instance("./output/Animations");
    return instance;
}

void AnimationProcessor::processAnimationStack(fbxsdk::FbxAnimStack *animationStack) {
    std::string stackName = Util::sanitizeString(animationStack->GetName());
    Util::createFolder(std::string("./output/AnimationStacks/") + stackName);
    int numLayers = animationStack->GetMemberCount(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimLayer::ClassId));

    Logger::log("layers: " + std::to_string(numLayers));

    //create folders for all layers in this animation stack and add them and their directories to
    // lists to use later when processing nodes

    for (int layerI = 0; layerI < numLayers; layerI++) {
        fbxsdk::FbxAnimLayer *layer = fbxsdk::FbxCast<fbxsdk::FbxAnimLayer>(animationStack->GetMember(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimLayer::ClassId), layerI));
        // fbxsdk::FbxAnimLayer *layer2 = animationStack->GetMember<FbxAnimLayer>(layerI);
        std::string layerName = Util::sanitizeString(layer->GetName());
        Logger::log(" - layer name: " + layerName);
        std::string layerDir = std::string("./output/AnimationStacks/") + stackName + std::string("/") + layerName;
        Util::createFolder(layerDir);

        animations.push_back(layer);
        animationDirectories.push_back(layerDir);
    }
}

// check if a node is animated, if so, the animation curve is processed and exported
void AnimationProcessor::processNodeForAnimation(fbxsdk::FbxNode *node) {
    for (int i = 0; i < animations.size(); i++) {

        // go through all animations, and check for animation curves that affect this node
        fbxsdk::FbxAnimCurve *posXCurve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
        fbxsdk::FbxAnimCurve *posYCurve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
        fbxsdk::FbxAnimCurve *posZCurve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

        fbxsdk::FbxAnimCurve *rotXCurve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
        fbxsdk::FbxAnimCurve *rotYCurve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
        fbxsdk::FbxAnimCurve *rotZCurve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

        bool hasPosAnim = posXCurve || posYCurve || posZCurve;
        bool hasRotAnim = rotXCurve || rotYCurve || rotZCurve;

        if (hasPosAnim || hasRotAnim) {
            if (hasPosAnim)
                saveKeyframes(node, posXCurve, posYCurve, posZCurve, i, "translation");
            if (hasRotAnim)
                saveKeyframes(node, rotXCurve, rotYCurve, rotZCurve, i, "rotation");
        }
    }
}

// exports the keyframes for a node and given animation curves
void AnimationProcessor::saveKeyframes(fbxsdk::FbxNode *node, fbxsdk::FbxAnimCurve *xCurve, fbxsdk::FbxAnimCurve *yCurve, fbxsdk::FbxAnimCurve *zCurve, int animationIndex, std::string filename) {
    if (!xCurve && !yCurve && !zCurve) {
        return;
    }
    std::cout << "Saving keyframes for " << filename << " on node " << node->GetName() << std::endl;

    std::vector<Keyframe> keyList;
    int keyframeCount = xCurve ? xCurve->KeyGetCount() : yCurve ? yCurve->KeyGetCount()
                                                     : zCurve   ? zCurve->KeyGetCount()
                                                                : 0;
    std::cout << "Keyframe count: " << keyframeCount << std::endl;

    //get values from all keys
    for (int k = 0; k < keyframeCount; k++) {
        Keyframe key = Keyframe();
        key.time = -1.f;
        FbxDouble3 defaultTranslation = node->LclTranslation.Get();

        if (xCurve) {
            fbxsdk::FbxAnimCurveKey xkey = xCurve->KeyGet(k);
            key.value[0] = xkey.GetValue();
            key.time = xkey.GetTime().GetSecondDouble();
            std::cout << "x - " << key.time << " - " << key.value[0] << std::endl;
        } else {
            key.value[0] = static_cast<float>(defaultTranslation[0]);
        }

        if (yCurve) {
            fbxsdk::FbxAnimCurveKey ykey = yCurve->KeyGet(k);
            key.value[1] = ykey.GetValue();
            key.time = ykey.GetTime().GetSecondDouble();
            std::cout << "y - " << key.time << " - " << key.value[1] << std::endl;
        } else {
            key.value[1] = static_cast<float>(defaultTranslation[1]);
        }

        if (zCurve) {
            fbxsdk::FbxAnimCurveKey zkey = zCurve->KeyGet(k);
            key.value[2] = zkey.GetValue();
            key.time = zkey.GetTime().GetSecondDouble();
            std::cout << "z - " << key.time << " - " << key.value[2] << std::endl;
        } else {
            key.value[2] = static_cast<float>(defaultTranslation[2]);
        }

        if (key.time == -1.f) {
            Logger::error("SOMETHING BIG WRONG");
        }

        // if (!(xkey.GetTime().GetSecondDouble() == ykey.GetTime().GetSecondDouble() && xkey.GetTime().GetSecondDouble() == zkey.GetTime().GetSecondDouble()))
        //     Logger::log("====================================================\n\n\n\n\n" +
        //                 std::string("Times not equal for all keyframes. \n") + std::string("This will cause artifacts in animations due to innacurate values"));

        keyList.push_back(key);
    }

    //save keys to file

    std::string animationCurveDir = animationDirectories[animationIndex] + "/" + Util::sanitizeString(node->GetName());
    Util::createFolder(animationCurveDir);
    std::string curveFname = Util::sanitizeString(animationCurveDir + "/" + filename);
    std::ofstream keyFile(curveFname, std::ios::out | std::ios::binary);
    if (!keyFile) {
        Logger::error("Could not open file " + curveFname);
    } else {
        keyFile.write((const char *)&keyList[0], keyList.size() * sizeof(Keyframe));
        keyFile.close();
    }
}
