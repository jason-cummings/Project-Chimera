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
    std::string fbxFilename;
    fbxsdk::FbxManager *manager;
    fbxsdk::FbxScene *scene;

    // data optimizers
    DataOptimizer meshOptimizer;
    DataOptimizer skinnedMeshOptimizer;
    DataOptimizer hitboxOptimizer;

    // iterates through all nodes and exports them
    void processNodes(fbxsdk::FbxNode *node, std::string parentDirectory);

    // processes Mesh and exports it
    void processMesh(fbxsdk::FbxMesh *mesh, std::string parentDirectory);

    //processes Mesh with skin deformer
    void processSkinnedMesh(fbxsdk::FbxMesh *mesh, std::string parentDirectory, std::vector<ControlPointBoneWeights> &boneWeights);

    // helper functions that retrieve information for
    glm::vec3 getNormal(fbxsdk::FbxMesh *mesh, int controlPointIndex, int vertexIndex);
    glm::vec2 getUV(fbxsdk::FbxMesh *mesh, int controlPointIndex, int vertexIndex);
    void getMaterial(fbxsdk::FbxSurfaceMaterial *material);
    void writeNodeTranslationInformtion(fbxsdk::FbxNode *node, std::string nodeDirectory);

    // if -h option is used when running program, these functions will be used to process only meshes and export them as hitboxes
    void processNodesForHitbox(fbxsdk::FbxNode *node, std::string parentDirectory);
    void processMeshForHitbox(fbxsdk::FbxMesh *mesh, std::string parentDirectory);

public:
    FbxParser(std::string filename);
    void init(bool forHitbox);
};

#endif