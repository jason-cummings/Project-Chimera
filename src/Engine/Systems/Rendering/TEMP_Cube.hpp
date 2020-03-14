#ifndef TEMPCUBE_H
#define TEMPCUBE_H

// #define GL_GLEXT_PROTOTYPES 1
// #include <SDL_opengl.h>

#include <GL/glew.h>

#include "Shader.hpp"
#include "../../GameObject.hpp"
#include "Mesh.hpp"

#define N_VERTEX_VALUES 19

class TempCube : public GameObject{
private:
	Mesh mesh;
    GLuint vbo, ibo;

    static const GLfloat vbo_data[];
    static const GLint ibo_data[];
    static const int n_vertices;

public:
    TempCube();
    void render();

    bool hasMesh() {return true;}
	Mesh * getMesh() {return &mesh;}

};

#endif