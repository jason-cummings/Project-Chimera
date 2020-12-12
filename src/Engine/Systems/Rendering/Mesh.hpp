#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "Material.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"

#define N_VERTEX_VALUES 19

class Mesh : public Renderable {
  private:
    GLuint vao, vertex_vao;
    GLuint vbo;
    GLuint ibo;
    int num_vertices;
    Material *material;

  public:
    Mesh() : vao(0), vbo(0), ibo(0), num_vertices(0), material(nullptr) {}
    Mesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in);
    ~Mesh();

    inline RenderableType getType() { return RenderableType::MESH; }

    void draw();
    void drawVerticesOnly();

    void setMaterial(Material *input);
    inline Material *getMaterial() { return material; }
};

#endif