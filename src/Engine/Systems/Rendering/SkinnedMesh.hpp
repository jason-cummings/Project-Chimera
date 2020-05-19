#ifndef SKINNED_MESH_H
#define SKINNED_MESH_H

#include <GL/glew.h>

#include "Renderable.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "./../Animation/Joint.hpp"

// #define N_FLOAT_ATTRIBUTES 23
// #define N_INT_ATTRIBUTES 4

#define N_INT_ATTRIBUTES 0
#define N_FLOAT_ATTRIBUTES 41

class SkinnedMesh : public Renderable {
private:
	GLuint vao, vertex_vao;
	GLuint vbo;
	GLuint ibo;
	int num_vertices;
	Material* material;
	JointList * joints;

public:
	SkinnedMesh() {vao=0;vbo=0;ibo=0;num_vertices=0;joints=NULL;}
	SkinnedMesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in, JointList * joints_in);
	~SkinnedMesh();

	inline RenderableType getType() { return RenderableType::SKINNED_MESH; }

	void draw();
	void drawVerticesOnly();

	void setMaterial( Material *input );
	inline Material * getMaterial() { return material; }
	inline JointList * getJointList() { return joints; }
};

#endif