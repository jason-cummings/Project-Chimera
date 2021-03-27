#ifndef SKELETONPROCESSOR_H
#define SKELETONPROCESSOR_H

#include <fbxsdk.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Joint {
    int parentIndex;
    std::string name;
    void *bonePointer = NULL;
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
        int lowestI = -1;
        float lowestWeight = 1.0;

        for (int i = 0; i < 4; i++) {
            if (weights[i] < lowestWeight) {
                lowestI = i;
                lowestWeight = weights[i];
            }
        }

        if (lowestI != -1) {
            indexes[lowestI] = index;
            weights[lowestI] = weight;
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

    std::vector<Joint> jointHierarchy;
    void processSkeletonHierarchyRecursive(fbxsdk::FbxNode *node, int depth, int index, int parentIndex);

    void associateJointsAndControlPoints(fbxsdk::FbxCluster *cluster, int jointIndex, std::vector<ControlPointBoneWeights> &boneWeights);
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
