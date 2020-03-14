#ifndef MESH_EXPORTER_H
#define MESH_EXPORTER_H

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <fstream>

template <class T>
struct VertexHasher
{
  std::size_t operator()(const T& k) const
  {
    return 1;
  }
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

};

template<class T>
class MeshExporter {
// private:
public:
	std::vector<T> VBO;
	std::vector<int> IBO;
	std::unordered_map<T, int, VertexHasher<T> > VboMap; // contains new indexing of vertices for polygons

public:
	MeshExporter();

	void addVertex(T vertex);
	void exportMesh(std::string directory); // exports the mesh to a file

	friend std::ostream &operator<<( std::ostream &output, const MeshExporter &me );

};

#endif