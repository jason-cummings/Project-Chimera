#include "SkeletonProcessor.hpp"

#include "../Util.hpp"

// helper function for processSkeletonHierarchy, recurses through all nodes and identifies joints
void SkeletonProcessor::processSkeletonHierarchyRecursive(fbxsdk::FbxNode *node, int depth, int index, int parentIndex) {
    if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) {
        Joint j;
        j.parentIndex = parentIndex;
        j.name = Util::sanitizeString(node->GetName());
        jointHierarchy.push_back(j);

        // debug
        std::cout << "Joint: " << j.name << ", Depth: " << depth << ", Index: " << index << std::endl;
    }

    for (int i = 0; i < node->GetChildCount(); i++) {
        fbxsdk::FbxNode *child = node->GetChild(i);
        processSkeletonHierarchyRecursive(child, depth + 1, jointHierarchy.size(), index);
    }
}

// provided the root node of the scene, this uses processSkeletonHierarchyRecursive to iterate through all nodes and identifies joints.
// a list of all joints is created, to be used for skeletal animation
void SkeletonProcessor::processSkeletonHierarchy(fbxsdk::FbxNode *node) {
    for (int i = 0; i < node->GetChildCount(); i++) {
        fbxsdk::FbxNode *child = node->GetChild(i);
        processSkeletonHierarchyRecursive(child, 0, jointHierarchy.size(), -1);
    }
}

// given a joint, iterates through control points that are affected by this joint and adds the weight to the boneWeights vector
void SkeletonProcessor::associateJointsAndControlPoints(FbxCluster *cluster, int jointIndex, std::vector<ControlPointBoneWeights> &boneWeights) {

    int numIndices = cluster->GetControlPointIndicesCount();
    for (int i = 0; i < numIndices; i++) {
        int controlPointIndex = (cluster->GetControlPointIndices())[i];
        float weight = (cluster->GetControlPointWeights())[i];

        //std::cout << "Index: " << controlPointIndex << ", Joint: " << jointIndex << ", Weight: " << weight << std::endl;

        boneWeights[controlPointIndex].addJointWeight(jointIndex, weight);
    }
}

// given a string, identifies the index of the joint with the specified name
int SkeletonProcessor::associateNameToJoint(std::string name) {
    for (int i = 0; i < jointHierarchy.size(); i++) {
        if (jointHierarchy[i].name.compare(name) == 0) {
            return i;
        }
    }
    std::cout << "ERROR: Could not match name with a joint index" << std::endl;
    return -1;
}

SkeletonProcessor &SkeletonProcessor::getInstance() {
    static SkeletonProcessor instance;
    return instance;
}

// process skin deformers for this node - used for skeletal animation
std::vector<ControlPointBoneWeights> SkeletonProcessor::processDeformers(fbxsdk::FbxNode *node) {
    FbxMesh *mesh = node->GetMesh();
    int numDeformers = mesh->GetDeformerCount(FbxDeformer::eSkin);

    std::vector<ControlPointBoneWeights> boneWeights;
    boneWeights.reserve(mesh->GetControlPointsCount());
    for (int i = 0; i < mesh->GetControlPointsCount(); i++)
        boneWeights[i] = ControlPointBoneWeights();

    //geometry transform

    // go through deformers for this node, if it is a skin deformer, associate all control points of the meshes to the appropriate joints
    for (int deformerI = 0; deformerI < numDeformers; deformerI++) {
        FbxSkin *skin = reinterpret_cast<FbxSkin *>(mesh->GetDeformer(deformerI, FbxDeformer::eSkin));

        if (skin) {
            int numClusters = skin->GetClusterCount();
            for (int clusterI = 0; clusterI < numClusters; clusterI++) {
                FbxCluster *cluster = skin->GetCluster(clusterI);
                int jointIndex = associateNameToJoint(Util::sanitizeString(cluster->GetLink()->GetName()));

                associateJointsAndControlPoints(cluster, jointIndex, boneWeights);
            }
        }
    }

    for (int i = 0; i < boneWeights.size(); i++) {
        boneWeights[i].rebalance();
    }

    return boneWeights;
}

// export the list of joints to a directory
void SkeletonProcessor::exportJointList(std::string directory) {
    std::ofstream jointListSizeFile(directory + "/JointListSize", std::ios::out | std::ios::binary);
    int size = jointHierarchy.size();
    jointListSizeFile.write((const char *)&size, sizeof(int));
    jointListSizeFile.close();

    for (int i = 0; i < jointHierarchy.size(); i++) {
        std::string indexDir = directory + "/" + std::to_string(i);
        Util::createFolder(indexDir);

        std::ofstream nameFile;
        nameFile.open(indexDir + "/name");
        nameFile << jointHierarchy[i].name;
        nameFile.close();

        std::ofstream parentIndexFile(indexDir + "/parent", std::ios::out | std::ios::binary);
        parentIndexFile.write((const char *)&(jointHierarchy[i].parentIndex), sizeof(int));
        parentIndexFile.close();
    }
}
