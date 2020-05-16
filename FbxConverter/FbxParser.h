#ifndef FBX_PARSER_H
#define FBX_PARSER_H

#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <exception>
#include <glm/glm.hpp>
#include "MeshExporter.h"
#include "DataOptimizer.h"
#include "Animation.h"
#include "FolderCreator.h"




struct Keyframe {
	double time;
	glm::vec3 value;
};

class FbxParser {
private:
	std::string fbx_filename;
	FbxManager* manager;
	FbxScene* scene;

	std::vector<FbxAnimLayer *> animations;
	std::vector<std::string> animation_directories;

	// data optimizers
	DataOptimizer mesh_optimizer;
	DataOptimizer skinned_mesh_optimizer;
	DataOptimizer material_optimizer; // not used yet but example usage
	DataOptimizer hitbox_optimizer;

	// processor for joints
	SkeletonProcessor skeleton_processor;

	// iterates through all nodes and exports them
	void processNodes(FbxNode * node, std::string depth, std::string parent_directory);
	
	// processes Mesh and exports it
	void processMesh(FbxMesh * mesh, std::string parent_directory);

	//processes Mesh with skin deformer
	void processSkinnedMesh(FbxMesh * mesh, std::string parent_directory,std::vector<ControlPointBoneWeights>& bone_weights);
	
	// helper functions that retrieve information for 
	glm::vec3 getNormal(FbxMesh * mesh, int control_point_index, int vertex_index);
	glm::vec2 getUV(FbxMesh * mesh,  int control_point_index, int vertex_index);

	// if -h option is used when running program, these functions will be used to process only meshes and export them as hitboxes
	void processNodesForHitbox(FbxNode * node, std::string depth, std::string parent_directory);
	void processMeshForHitbox(FbxMesh * mesh, std::string parent_directory);

	// processes animation data
	void processAnimationStack(FbxAnimStack * animation_stack);
	void processNodeForAnimation(FbxNode * node);
	void saveKeyframes(FbxNode * node, FbxAnimCurve* x_curve, FbxAnimCurve* y_curve, FbxAnimCurve* z_curve, int animation_index, std::string filename);

public:
	FbxParser(std::string filename);
	void init(bool for_hitbox);

};

#endif