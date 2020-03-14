#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "Shader.hpp"

#define N_VERTEX_VALUES 19

class Mesh {
private:
	GLuint vbo;
	GLuint ibo;
	int num_vertices;

public:
	Mesh() {vbo=0;ibo=0;num_vertices=0;}
	Mesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in);
	~Mesh();

	void setUpDraw();
	void cleanUpDraw();

	void slowDraw();
	void quickDraw();



	void bindMaterial();
};

#endif