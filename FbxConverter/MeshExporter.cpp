#include "MeshExporter.h"


bool operator==(const Vertex& v1, const Vertex& v2) {
	return (v1.position == v2.position) && 
		(v1.normal == v2.normal) && 
		(v1.uv == v2.uv);
}

template <class T>
MeshExporter<T>::MeshExporter(){}

template <class T>
void MeshExporter<T>::addVertex(T vertex) {
	bool found = false;
	int i = 0;

	while(!found && i < VBO.size()) {
		if(vertex == VBO[i]) {
			found = true;
		}
		else {
			i++;
		}
	}

	if(found) {
		// if key found, add index of vertex to IBO
		IBO.push_back(i);
	}
	else {
		//if key not found, add vertex to VBO, then add vertex and index to VboMap and IBO
		int index = VBO.size();
		VBO.push_back(vertex);
		IBO.push_back(index);
		VboMap[vertex] = index;
	}

	// // check if vertex is already in map
	// if(VboMap.find(vertex) != VboMap.end()) {
	// 	// if key found, add index of vertex to IBO
	// 	IBO.push_back(VboMap[vertex]);
	// }
	// else {
	// 	//if key not found, add vertex to VBO, then add vertex and index to VboMap and IBO
	// 	int index = VBO.size();
	// 	VBO.push_back(vertex);
	// 	IBO.push_back(index);
	// 	VboMap[vertex] = index;
	// }
}

template <class T>
void MeshExporter<T>::exportMesh(std::string directory) {
	std::ofstream vboFile (directory + "VBO", std::ios::out | std::ios::binary);
	vboFile.write((const char *)&VBO[0],VBO.size() * sizeof(T));
	vboFile.close();

	std::ofstream iboFile (directory + "IBO", std::ios::out | std::ios::binary);
	iboFile.write((const char *)&IBO[0],IBO.size() * sizeof(int));
	iboFile.close();
}


std::ostream &operator<<( std::ostream &output, const MeshExporter<Vertex> &me ) { 
    for(int i = 0; i < me.VBO.size(); i++) {
	output << me.VBO[i] << std::endl;
	}	

	for(int i = 0; i < me.IBO.size(); i++) {
		output << me.IBO[i] << std::endl;
	}
    return output;            
}  

// tell compiler to create versions of MeshExporter for the different types of Vertex
template class MeshExporter<Vertex>;



// int main() {
// 	MeshExporter<Vertex> me = MeshExporter<Vertex>();

// 	Vertex v = Vertex();
// 	v.position = glm::vec4(3.0,0,0,0);
// 	me.addVertex(v);
// 	me.addVertex(v);
// 	me.addVertex(v);
// 	v.position = glm::vec4(3.0,1.0,0,0);
// 	me.addVertex(v);
// 	me.exportMesh(std::string("./"));
// 	std::cout << me << std::endl;

// 	return 0;
// }