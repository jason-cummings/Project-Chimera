#ifndef SKELETONPROCESSOR_H
#define SKELETONPROCESSOR_H

#include <fbxsdk.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Joint {
    int parent_index;
    std::string name;
    void *bone_pointer = NULL;
};

// keeps track of the bones affecting a vertex and their weights. Has functionality to limit number of joints that can affect a vertex, and only keeps the 4 highest weights
struct ControlPointBoneWeights {

    int indexes[4] = {0, 0, 0, 0};
    float weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    ControlPointBoneWeights() {
        for (int i = 0; i < 4; i++) {
            indexes[i] = 0;
            weights[i] = 0.0f;
        }
    }

    // theoretically, sum of all weights should be 1, for the case that there are more
    //  than 4 bones affecting this control point, only first 4 are counted, and the rebalance
    // function resets the weights so that the sum is 1
    void rebalance() {
        float sum = weights[0] + weights[1] + weights[2] + weights[3];

        weights[0] = weights[0] / sum;
        weights[1] = weights[1] / sum;
        weights[2] = weights[2] / sum;
        weights[3] = weights[3] / sum;
    }

    void addJointWeight(int index, float weight) {
        int lowest_i = -1;
        float lowest_weight = 1.0;

        for (int i = 0; i < 4; i++) {
            if (weights[i] < lowest_weight) {
                lowest_i = i;
                lowest_weight = weights[i];
            }
        }

        if (lowest_i != -1) {
            indexes[lowest_i] = index;
            weights[lowest_i] = weight;
        } else {
            std::cout << "Ignoring this joint weight, weight: " << weight << std::endl;
            std::cout << indexes[0] << ", " << weights[0] << std::endl;
            std::cout << indexes[1] << ", " << weights[1] << std::endl;
            std::cout << indexes[2] << ", " << weights[2] << std::endl;
            std::cout << indexes[3] << ", " << weights[3] << std::endl;
        }
    }
};

//maintains a list of all joints in the scene, and is used to associate joints to vertices in meshes, and export the list of joints to files
class SkeletonProcessor {

    std::vector<Joint> joint_hierarchy;
    void processSkeletonHierarchyRecursive(fbxsdk::FbxNode *node, int depth, int index, int parentIndex);

    void associateJointsAndControlPoints(fbxsdk::FbxCluster *cluster, int joint_index, std::vector<ControlPointBoneWeights> &bone_weights);
    int associateNameToJoint(std::string name);

    /** Singleton constructor */
    SkeletonProcessor(){};

public:
    /** @return singleton instance */
    static SkeletonProcessor &getInstance();

    /**
     * The thing
     * @param node 
     */
    void processSkeletonHierarchy(fbxsdk::FbxNode *node);

    std::vector<ControlPointBoneWeights> processDeformers(fbxsdk::FbxNode *node);

    void exportJointList(std::string directory);
};

#endif
