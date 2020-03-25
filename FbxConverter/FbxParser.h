#ifndef FBX_PARSER_H
#define FBX_PARSER_H

#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <exception>
#include <glm/glm.hpp>
#include "MeshExporter.h"
#include "DataOptimizer.h"
#include "FolderCreator.h"

class FbxParser {
private:
	std::string fbx_filename;
	FbxManager* manager;
	FbxScene* scene;

	DataOptimizer mesh_optimizer;
	DataOptimizer material_optimizer; // not used yet but example usage
public:
	FbxParser(std::string filename);
	void init();
	void processNodes(FbxNode * node, std::string depth, std::string parent_directory);
	void processMesh(FbxMesh * mesh, std::string parent_directory);
	glm::vec3 getNormal(FbxMesh * mesh, int control_point_index, int vertex_index);
	glm::vec2 getUV(FbxMesh * mesh,  int control_point_index, int vertex_index);
};

#endif