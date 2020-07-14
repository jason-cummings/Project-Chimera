#ifndef OVERLAYMESH_H
#define OVERLAYMESH_H

#include <GL/glew.h>

#include "Renderable.hpp"
#include "Shader.hpp"
#include "Material.hpp"

class OverlayMesh : public Renderable {
private:
    GLuint vao; //, vbo, ibo;
    int num_vertices;
    Material *material;

	int z_level;

public:
	OverlayMesh(): vao(0), num_vertices(0), material(nullptr), z_level(100) {}
	OverlayMesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in);
	OverlayMesh(GLuint vao_in, int num_vertices_in);
	~OverlayMesh();

	inline RenderableType getType() { return RenderableType::OVERLAY; }

	void draw();

	void setMaterial( Material *input );
	inline Material * getMaterial() { return material; }

    inline void setZLevel( int z_in ) { z_level = z_in; }
    inline int getZLevel() { return z_level; }
};

#endif