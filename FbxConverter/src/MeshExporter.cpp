#include "MeshExporter.hpp"

// #include "FilesystemWrapper.hpp"

// operators needed for the Vertex types
bool operator==(const Vertex &v1, const Vertex &v2) {
    return (v1.position == v2.position) &&
           (v1.normal == v2.normal) &&
           (v1.uv == v2.uv);
}

bool operator==(const SkinnedVertex &v1, const SkinnedVertex &v2) {
    return (v1.position == v2.position) &&
           (v1.normal == v2.normal) &&
           (v1.uv == v2.uv);
}

bool operator==(const MinimalVertex &v1, const MinimalVertex &v2) {
    return (v1.position == v2.position);
}

template <class T>
MeshExporter<T>::MeshExporter() {}

// add vertex for exporting
template <class T>
void MeshExporter<T>::addVertex(T vertex) {
    bool found = false;
    int i = 0;

    while (!found && i < VBO.size()) {
        if (vertex == VBO[i]) {
            found = true;
        } else {
            i++;
        }
    }

    if (found) {
        // if key found, add index of vertex to IBO
        IBO.push_back(i);
    } else {
        //if key not found, add vertex to VBO, then add vertex and index to VboMap and IBO
        int index = VBO.size();
        VBO.push_back(vertex);
        IBO.push_back(index);
    }
}

// export VBO and IBO to files
template <class T>
void MeshExporter<T>::exportMesh(std::string directory) {
    std::ofstream vboFile(directory + "/VBO", std::ios::out | std::ios::binary);
    vboFile.write((const char *)&VBO[0], VBO.size() * sizeof(T));
    vboFile.close();

    std::ofstream iboFile(directory + "/IBO", std::ios::out | std::ios::binary);
    iboFile.write((const char *)&IBO[0], IBO.size() * sizeof(int));
    iboFile.close();
}

// debug operator for printing ibo and vbo
std::ostream &operator<<(std::ostream &output, const MeshExporter<Vertex> &me) {
    for (int i = 0; i < me.VBO.size(); i++) {
        output << me.VBO[i] << std::endl;
    }

    for (int i = 0; i < me.IBO.size(); i++) {
        output << me.IBO[i] << std::endl;
    }
    return output;
}

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

// tell compiler to create versions of MeshExporter for the different types of Vertex
template class MeshExporter<Vertex>;
template class MeshExporter<MinimalVertex>;
template class MeshExporter<SkinnedVertex>;
