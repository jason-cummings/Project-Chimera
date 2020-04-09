#include "Animation.h"


void SkeletonProcessor::processSkeletonHierarchyRecursive(FbxNode * node, int depth, int index, int parentIndex) {
	if(node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
		Joint j;
		j.parent_index = parentIndex;
		j.name = node->GetName();
		joint_hierarchy.push_back(j);

		// debug
		std::cout << "Joint: " << j.name << ", Depth: " << depth << std::endl;
	}

	for(int i = 0; i < node->GetChildCount(); i++) {
		FbxNode* child = node->GetChild(i);
		processSkeletonHierarchyRecursive(child, depth+1, joint_hierarchy.size(), index);
	}
}


void SkeletonProcessor::processSkeletonHierarchy(FbxNode * node) {
	for(int i = 0; i < node->GetChildCount(); i++) {
		FbxNode* child = node->GetChild(i);
		processSkeletonHierarchyRecursive(child,0,joint_hierarchy.size(),-1);
	}
}

void SkeletonProcessor::associateJointsAndControlPoints(FbxCluster* cluster, int joint_index, std::vector<ControlPointBoneWeights>& bone_weights) {
	//std::cout << "\nAssociating joint to control points" << std::endl;
	int num_indices = cluster->GetControlPointIndicesCount();
	for(int i = 0; i < num_indices; i++) {
		int control_point_index = (cluster->GetControlPointIndices())[i];
		float weight = (cluster->GetControlPointWeights())[i];
		
		//std::cout << "Index: " << control_point_index << ", Joint: " << joint_index << ", Weight: " << weight << std::endl;
		
		bone_weights[control_point_index].addJointWeight(joint_index,weight);
	}

}

int SkeletonProcessor::associateNameToJoint(std::string name) {
	for(int i = 0; i < joint_hierarchy.size(); i++) {
		if(joint_hierarchy[i].name.compare(name) == 0) {
			return i;
		}
	}
	std::cout << "ERROR: Could not match name with a joint index" << std::endl;
	return -1;
}

std::vector<ControlPointBoneWeights> SkeletonProcessor::processDeformers(FbxNode* node) {
	FbxMesh * mesh = node->GetMesh();
	int num_deformers = mesh->GetDeformerCount(FbxDeformer::eSkin);

	std::vector<ControlPointBoneWeights> bone_weights;
	bone_weights.reserve(mesh->GetControlPointsCount());
	for(int i = 0; i < mesh->GetControlPointsCount(); i++)
		bone_weights[i] = ControlPointBoneWeights();
	



	//geometry transform

	

	for(int deformer_i = 0; deformer_i < num_deformers; deformer_i++) {
		FbxSkin * skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformer_i, FbxDeformer::eSkin));

		if(skin) {
			int num_clusters = skin->GetClusterCount();
			for(int cluster_i = 0; cluster_i < num_clusters; cluster_i++) {
				FbxCluster * cluster = skin->GetCluster(cluster_i);
				int joint_index = associateNameToJoint(cluster->GetLink()->GetName());

				associateJointsAndControlPoints(cluster,joint_index,bone_weights);

			}
		}
	}

	for(int i = 0; i < bone_weights.size(); i++) {
		bone_weights[i].rebalance();
	}

	return bone_weights;
}

void SkeletonProcessor::exportJointList(std::string directory) {
		std::ofstream joint_list_file (directory + "/Joints", std::ios::out | std::ios::binary);
		joint_list_file.write((const char *)&joint_hierarchy[0],joint_hierarchy.size() * sizeof(Joint));
		joint_list_file.close();
}

