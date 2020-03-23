#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include "Mesh.hpp"
#include "./../../Asset.hpp"

//Search Directory for vbo or ibo files and if there, read them in. If files not found, STD_Runtime_Error("String with error")

class MeshFactory{
public:
	//Takes directory as parameter creates assets for the meshes.
	static Mesh* createBasicMesh(std::string input_directory);
private:
};





#endif