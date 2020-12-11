#ifndef FBX_PARSER_H
#define FBX_PARSER_H

#include "Animation.h"
#include "DataOptimizer.h"
#include "FolderCreator.h"
#include "MaterialProcessor.hpp"
#include "MeshExporter.h"
#include <exception>
#include <fbxsdk.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

struct Keyframe {
    double time;
    glm::vec3 value;
};

class FbxParser {
  private:
    std::string fbx_filename;
    fbxsdk::FbxManager *manager;
    fbxsdk::FbxScene *scene;

    std::vector<fbxsdk::FbxAnimLayer *> animations;
    std::vector<std::string> animation_directories;

    // data optimizers
    DataOptimizer mesh_optimizer;
    DataOptimizer skinned_mesh_optimizer;
    DataOptimizer hitbox_optimizer;

    MaterialProcessor material_processor;

    // processor for joints
    SkeletonProcessor skeleton_processor;

    // iterates through all nodes and exports them
    void processNodes(fbxsdk::FbxNode *node, std::string depth, std::string parent_directory);

    // processes Mesh and exports it
    void processMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory);

    //processes Mesh with skin deformer
    void processSkinnedMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory, std::vector<ControlPointBoneWeights> &bone_weights);

    // helper functions that retrieve information for
    glm::vec3 getNormal(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index);
    glm::vec2 getUV(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index);
    void getMaterial(fbxsdk::FbxSurfaceMaterial *material);

    // if -h option is used when running program, these functions will be used to process only meshes and export them as hitboxes
    void processNodesForHitbox(fbxsdk::FbxNode *node, std::string depth, std::string parent_directory);
    void processMeshForHitbox(fbxsdk::FbxMesh *mesh, std::string parent_directory);

    // processes animation data
    void processAnimationStack(fbxsdk::FbxAnimStack *animation_stack);
    void processNodeForAnimation(fbxsdk::FbxNode *node);
    void saveKeyframes(fbxsdk::FbxNode *node, fbxsdk::FbxAnimCurve *x_curve, fbxsdk::FbxAnimCurve *y_curve, fbxsdk::FbxAnimCurve *z_curve, int animation_index, std::string filename);

  public:
    FbxParser(std::string filename);
    void init(bool for_hitbox);
};

#endif