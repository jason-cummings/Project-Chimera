#ifndef OVERLAYMESH_H
#define OVERLAYMESH_H

#include <GL/glew.h>

#include "Shader.hpp"
#include "Material.hpp"

class OverlayMesh {
private:
    GLuint vao, vbo, ibo;
    int num_vertices;
    Material *material;

public:
	OverlayMesh(): vao(0), vbo(0), ibo(0), num_vertices(0), material(nullptr) {}
	OverlayMesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in);
	~OverlayMesh();

	void draw();

	void setMaterial( Material *input );
	inline Material * getMaterial() { return material; }
};

#endif