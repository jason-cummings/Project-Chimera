#ifndef FBX_PARSER_H
#define FBX_PARSER_H

#include <exception>
#include <fbxsdk.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "DataOptimizer.hpp"
#include "MeshExporter.hpp"
#include "processors/MaterialProcessor.hpp"
#include "processors/SkeletonProcessor.hpp"

class FbxParser {
private:
    std::string fbx_filename;
    fbxsdk::FbxManager *manager;
    fbxsdk::FbxScene *scene;

    // data optimizers
    DataOptimizer mesh_optimizer;
    DataOptimizer skinned_mesh_optimizer;
    DataOptimizer hitbox_optimizer;

    // iterates through all nodes and exports them
    void processNodes(fbxsdk::FbxNode *node, std::string parent_directory);

    // processes Mesh and exports it
    void processMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory);

    //processes Mesh with skin deformer
    void processSkinnedMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory, std::vector<ControlPointBoneWeights> &bone_weights);

    // helper functions that retrieve information for
    glm::vec3 getNormal(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index);
    glm::vec2 getUV(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index);
    void getMaterial(fbxsdk::FbxSurfaceMaterial *material);
    void writeNodeTranslationInformtion(fbxsdk::FbxNode *node, std::string node_directory);

    // if -h option is used when running program, these functions will be used to process only meshes and export them as hitboxes
    void processNodesForHitbox(fbxsdk::FbxNode *node, std::string parent_directory);
    void processMeshForHitbox(fbxsdk::FbxMesh *mesh, std::string parent_directory);

public:
    FbxParser(std::string filename);
    void init(bool for_hitbox);
};

#endif