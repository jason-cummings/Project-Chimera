#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

#include "Mesh.hpp"
#include "SkinnedMesh.hpp"
#include "../../Utilities/Asset.hpp"
#include "../../Utilities/FilesystemWrapper.hpp"

//Search Directory for vbo or ibo files and if there, read them in. If files not found, STD_Runtime_Error("String with error")

class MeshFactory{
public:
	//Takes directory as parameter creates assets for the meshes.
	static Mesh* createBasicMesh(std::string input_directory);
	static Mesh* createBasicMesh(fs::path input_directory);
	static SkinnedMesh* createSkinnedMesh(std::string input_directory, JointList * joint_list);
	static SkinnedMesh* createSkinnedMesh(fs::path input_directory, JointList * joint_list);
private:
	static GLuint createVBO(Asset& asset);
	static GLuint createIBO(Asset& asset);
};





#endif