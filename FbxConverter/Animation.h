#ifndef ANIMATION_H
#define ANIMATION_H

#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>

struct Joint {
	int parent_index;
	std::string name;
	void * bone_pointer = NULL;

};

struct ControlPointBoneWeights {

	int indexes[4] = {0,0,0,0};
	float weights[4] = {0.0f,0.0f,0.0f,0.0f};
	ControlPointBoneWeights() {
		for(int i = 0; i < 4; i++) {
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

		for(int i = 0; i < 4; i++) {
			if(weights[i] < lowest_weight) {
				lowest_i = i;
				lowest_weight = weights[i];
			}
		}

		if(lowest_i != -1) {
			indexes[lowest_i] = index;
			weights[lowest_i] = weight;
		}
		else {
			std::cout << "Ignoring this joint weight, weight: " << weight << std::endl;
			std::cout << indexes[0] << ", " << weights[0] << std::endl;
			std::cout << indexes[1] << ", " << weights[1] << std::endl;
			std::cout << indexes[2] << ", " << weights[2] << std::endl;
			std::cout << indexes[3] << ", " << weights[3] << std::endl;
		}
	}
};

//maintains a list of all joints in the scene
class SkeletonProcessor {

	std::vector<Joint> joint_hierarchy;
	void processSkeletonHierarchyRecursive(FbxNode * node, int depth, int index, int parentIndex);

	void associateJointsAndControlPoints(FbxCluster* cluster, int joint_index, std::vector<ControlPointBoneWeights>& bone_weights);
	int associateNameToJoint(std::string name);
	

public:
	// pass in root node of the scene
	void processSkeletonHierarchy(FbxNode * node);
	std::vector<ControlPointBoneWeights> processDeformers(FbxNode* node);
	void exportJointList(std::string directory);

};

#endif
