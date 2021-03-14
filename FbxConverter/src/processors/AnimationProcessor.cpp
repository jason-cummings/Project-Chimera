#include "AnimationProcessor.hpp"

#include <fstream>

#include "../Logger.hpp"

AnimationProcessor::AnimationProcessor(std::string base_dir) : animation_optimizer(base_dir) {
}

AnimationProcessor &AnimationProcessor::getInstance() {
    static AnimationProcessor instance("./output/Animations");
    return instance;
}

void AnimationProcessor::processAnimationStack(fbxsdk::FbxAnimStack *animation_stack) {
    std::string stack_name = Util::sanitizeString(animation_stack->GetName());
    Util::createFolder(std::string("./output/AnimationStacks/") + stack_name);
    int num_layers = animation_stack->GetMemberCount(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimLayer::ClassId));

    Logger::log("layers: " + std::to_string(num_layers));

    //create folders for all layers in this animation stack and add them and their directories to
    // lists to use later when processing nodes

    for (int layer_i = 0; layer_i < num_layers; layer_i++) {
        fbxsdk::FbxAnimLayer *layer = fbxsdk::FbxCast<fbxsdk::FbxAnimLayer>(animation_stack->GetMember(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimLayer::ClassId), layer_i));
        // fbxsdk::FbxAnimLayer *layer2 = animation_stack->GetMember<FbxAnimLayer>(layer_i);
        std::string layer_name = Util::sanitizeString(layer->GetName());
        Logger::log(" - layer name: " + layer_name);
        std::string layer_dir = std::string("./output/AnimationStacks/") + stack_name + std::string("/") + layer_name;
        Util::createFolder(layer_dir);

        animations.push_back(layer);
        animation_directories.push_back(layer_dir);
    }
}

// check if a node is animated, if so, the animation curve is processed and exported
void AnimationProcessor::processNodeForAnimation(fbxsdk::FbxNode *node) {
    for (int i = 0; i < animations.size(); i++) {

        // go through all animations, and check for animation curves that affect this node
        fbxsdk::FbxAnimCurve *pos_x_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
        fbxsdk::FbxAnimCurve *pos_y_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
        fbxsdk::FbxAnimCurve *pos_z_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

        fbxsdk::FbxAnimCurve *rot_x_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
        fbxsdk::FbxAnimCurve *rot_y_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
        fbxsdk::FbxAnimCurve *rot_z_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

        bool has_pos_anim = pos_x_curve || pos_y_curve || pos_z_curve;
        bool has_rot_anim = rot_x_curve || rot_y_curve || rot_z_curve;

        if (has_pos_anim || has_rot_anim) {
            if (has_pos_anim)
                saveKeyframes(node, pos_x_curve, pos_y_curve, pos_z_curve, i, "translation");
            if (has_rot_anim)
                saveKeyframes(node, rot_x_curve, rot_y_curve, rot_z_curve, i, "rotation");
        }
    }
}

// exports the keyframes for a node and given animation curves
void AnimationProcessor::saveKeyframes(fbxsdk::FbxNode *node, fbxsdk::FbxAnimCurve *x_curve, fbxsdk::FbxAnimCurve *y_curve, fbxsdk::FbxAnimCurve *z_curve, int animation_index, std::string filename) {
    if (!x_curve && !y_curve && !z_curve) {
        return;
    }
    std::cout << "Saving keyframes for " << filename << " on node " << node->GetName() << std::endl;

    std::vector<Keyframe> key_list;
    int keyframe_count = x_curve ? x_curve->KeyGetCount() : y_curve ? y_curve->KeyGetCount()
                                                        : z_curve   ? z_curve->KeyGetCount()
                                                                    : 0;
    std::cout << "Keyframe count: " << keyframe_count << std::endl;

    //get values from all keys
    for (int k = 0; k < keyframe_count; k++) {
        Keyframe key = Keyframe();
        key.time = -1.f;
        FbxDouble3 default_translation = node->LclTranslation.Get();

        if (x_curve) {
            fbxsdk::FbxAnimCurveKey xkey = x_curve->KeyGet(k);
            key.value[0] = xkey.GetValue();
            key.time = xkey.GetTime().GetSecondDouble();
            std::cout << "x - " << key.time << " - " << key.value[0] << std::endl;
        } else {
            key.value[0] = static_cast<float>(default_translation[0]);
        }

        if (y_curve) {
            fbxsdk::FbxAnimCurveKey ykey = y_curve->KeyGet(k);
            key.value[1] = ykey.GetValue();
            key.time = ykey.GetTime().GetSecondDouble();
            std::cout << "y - " << key.time << " - " << key.value[1] << std::endl;
        } else {
            key.value[1] = static_cast<float>(default_translation[1]);
        }

        if (z_curve) {
            fbxsdk::FbxAnimCurveKey zkey = z_curve->KeyGet(k);
            key.value[2] = zkey.GetValue();
            key.time = zkey.GetTime().GetSecondDouble();
            std::cout << "z - " << key.time << " - " << key.value[2] << std::endl;
        } else {
            key.value[2] = static_cast<float>(default_translation[2]);
        }

        if (key.time == -1.f) {
            Logger::error("SOMETHING BIG WRONG");
        }

        // if (!(xkey.GetTime().GetSecondDouble() == ykey.GetTime().GetSecondDouble() && xkey.GetTime().GetSecondDouble() == zkey.GetTime().GetSecondDouble()))
        //     Logger::log("====================================================\n\n\n\n\n" +
        //                 std::string("Times not equal for all keyframes. \n") + std::string("This will cause artifacts in animations due to innacurate values"));

        key_list.push_back(key);
    }

    //save keys to file

    std::string animation_curve_dir = animation_directories[animation_index] + "/" + Util::sanitizeString(node->GetName());
    Util::createFolder(animation_curve_dir);
    std::string curve_fname = Util::sanitizeString(animation_curve_dir + "/" + filename);
    std::ofstream key_file(curve_fname, std::ios::out | std::ios::binary);
    if (!key_file) {
        Logger::error("Could not open file " + curve_fname);
    } else {
        key_file.write((const char *)&key_list[0], key_list.size() * sizeof(Keyframe));
        key_file.close();
    }
}
