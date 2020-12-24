#include "Animation.hpp"

#include "Util.hpp"

// helper function for processSkeletonHierarchy, recurses through all nodes and identifies joints
void SkeletonProcessor::processSkeletonHierarchyRecursive(fbxsdk::FbxNode *node, int depth, int index, int parentIndex) {
    if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) {
        Joint j;
        j.parent_index = parentIndex;
        j.name = Util::sanitizeString(node->GetName());
        joint_hierarchy.push_back(j);

        // debug
        std::cout << "Joint: " << j.name << ", Depth: " << depth << ", Index: " << index << std::endl;
    }

    for (int i = 0; i < node->GetChildCount(); i++) {
        fbxsdk::FbxNode *child = node->GetChild(i);
        processSkeletonHierarchyRecursive(child, depth + 1, joint_hierarchy.size(), index);
    }
}

// provided the root node of the scene, this uses processSkeletonHierarchyRecursive to iterate through all nodes and identifies joints.
// a list of all joints is created, to be used for skeletal animation
void SkeletonProcessor::processSkeletonHierarchy(fbxsdk::FbxNode *node) {
    for (int i = 0; i < node->GetChildCount(); i++) {
        fbxsdk::FbxNode *child = node->GetChild(i);
        processSkeletonHierarchyRecursive(child, 0, joint_hierarchy.size(), -1);
    }
}

// given a joint, iterates through control points that are affected by this joint and adds the weight to the bone_weights vector
void SkeletonProcessor::associateJointsAndControlPoints(FbxCluster *cluster, int joint_index, std::vector<ControlPointBoneWeights> &bone_weights) {

    int num_indices = cluster->GetControlPointIndicesCount();
    for (int i = 0; i < num_indices; i++) {
        int control_point_index = (cluster->GetControlPointIndices())[i];
        float weight = (cluster->GetControlPointWeights())[i];

        //std::cout << "Index: " << control_point_index << ", Joint: " << joint_index << ", Weight: " << weight << std::endl;

        bone_weights[control_point_index].addJointWeight(joint_index, weight);
    }
}

// given a string, identifies the index of the joint with the specified name
int SkeletonProcessor::associateNameToJoint(std::string name) {
    for (int i = 0; i < joint_hierarchy.size(); i++) {
        if (joint_hierarchy[i].name.compare(name) == 0) {
            return i;
        }
    }
    std::cout << "ERROR: Could not match name with a joint index" << std::endl;
    return -1;
}

// process skin deformers for this node - used for skeletal animation
std::vector<ControlPointBoneWeights> SkeletonProcessor::processDeformers(fbxsdk::FbxNode *node) {
    FbxMesh *mesh = node->GetMesh();
    int num_deformers = mesh->GetDeformerCount(FbxDeformer::eSkin);

    std::vector<ControlPointBoneWeights> bone_weights;
    bone_weights.reserve(mesh->GetControlPointsCount());
    for (int i = 0; i < mesh->GetControlPointsCount(); i++)
        bone_weights[i] = ControlPointBoneWeights();

    //geometry transform

    // go through deformers for this node, if it is a skin deformer, associate all control points of the meshes to the appropriate joints
    for (int deformer_i = 0; deformer_i < num_deformers; deformer_i++) {
        FbxSkin *skin = reinterpret_cast<FbxSkin *>(mesh->GetDeformer(deformer_i, FbxDeformer::eSkin));

        if (skin) {
            int num_clusters = skin->GetClusterCount();
            for (int cluster_i = 0; cluster_i < num_clusters; cluster_i++) {
                FbxCluster *cluster = skin->GetCluster(cluster_i);
                int joint_index = associateNameToJoint(Util::sanitizeString(cluster->GetLink()->GetName()));

                associateJointsAndControlPoints(cluster, joint_index, bone_weights);
            }
        }
    }

    for (int i = 0; i < bone_weights.size(); i++) {
        bone_weights[i].rebalance();
    }

    return bone_weights;
}

// export the list of joints to a directory
void SkeletonProcessor::exportJointList(std::string directory) {
    std::ofstream joint_list_size_file(directory + "/JointListSize", std::ios::out | std::ios::binary);
    int size = joint_hierarchy.size();
    joint_list_size_file.write((const char *)&size, sizeof(int));
    joint_list_size_file.close();

    for (int i = 0; i < joint_hierarchy.size(); i++) {
        std::string index_dir = directory + "/" + std::to_string(i);
        Util::createFolder(index_dir);

        std::ofstream name_file;
        name_file.open(index_dir + "/name");
        name_file << joint_hierarchy[i].name;
        name_file.close();

        std::ofstream parent_index_file(index_dir + "/parent", std::ios::out | std::ios::binary);
        parent_index_file.write((const char *)&(joint_hierarchy[i].parent_index), sizeof(int));
        parent_index_file.close();
    }
}
