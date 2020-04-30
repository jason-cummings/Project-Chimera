#ifndef MESH_EXPORTER_H
#define MESH_EXPORTER_H

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <boost/filesystem.hpp>
#include <exception>


struct MinimalVertex {
	glm::vec3 position;

	friend bool operator==(const MinimalVertex& v1, const MinimalVertex& v2);
};

struct Vertex {
	glm::vec4 position;
	glm::vec4 rgba = glm::vec4(0.3f,0.3f,0.3f,1.0f);
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 uv;


	bool operator<(const Vertex& v2) const{
		//return *this == v2;
    	return position[0] < v2.position[0];
    }

	friend bool operator==(const Vertex& v1, const Vertex& v2);
	friend std::ostream &operator<<( std::ostream &output, const Vertex &v ) { 
        output << "Position : " << glm::to_string(v.position) << ", Normal : " << glm::to_string(v.normal) << ", UV : " << glm::to_string(v.uv);
        return output;            
    }
};

struct NormalMappedVertex {

};

// stub for later use
struct SkinnedVertex {
	glm::vec4 position;
	glm::vec4 rgba = glm::vec4(0.3f,0.3f,0.3f,1.0f);
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 uv;

	// int joint_index_0 = 0;
	// float joint_weight_0 = 0.0f;
	// int joint_index_1 = 0;
	// float joint_weight_1 = 0.0f;
	// int joint_index_2 = 0;
	// float joint_weight_2 = 0.0f;
	// int joint_index_3 = 0;
	// float joint_weight_3 = 0.0f;

	float weight_array[22];/* = {0.0,0.0,0.0,0.0,
							 0.0,0.0,0.0,0.0,
							 0.0,0.0,0.0,0.0,
							 0.0,0.0,0.0,0.0,
							 0.0,0.0,0.0,0.0,
							 0.0,0.0};*/


	// bool operator<(const Vertex& v2) const{
	// 	//return *this == v2;
 //    	return position[0] < v2.position[0];
 //    }

	friend bool operator==(const SkinnedVertex& v1, const SkinnedVertex& v2);
	friend std::ostream &operator<<( std::ostream &output, const SkinnedVertex &v ) { 
        output << "Position : " << glm::to_string(v.position) << ", Normal : " << glm::to_string(v.normal) << ", UV : " << glm::to_string(v.uv);
        return output;            
    }
};

template<class T>
class MeshExporter {
// private:
public:
	std::vector<T> VBO;
	std::vector<int> IBO;

public:
	MeshExporter();

	void addVertex(T vertex);
	void exportMesh(std::string directory); // exports the mesh to a file

	friend std::ostream &operator<<( std::ostream &output, const MeshExporter &me );

};

#endif