#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "Shader.hpp"
#include "Material.hpp"

#define N_VERTEX_VALUES 19

class Mesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	int num_vertices;
	Material *material;

public:
	Mesh(): vao(0), vbo(0), ibo(0), num_vertices(0), material(nullptr) {}
	Mesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in);
	~Mesh();

	void draw();

	void setMaterial( Material *input );
	inline Material * getMaterial() { return material; }
};

#endif