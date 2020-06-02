#ifndef RENDER_UTILS_HPP
#define RENDER_UTILS_HPP

#include <iostream>
#include <GL/glew.h>
#include "Shader.hpp"


class RenderUtils {
private:
	//VAO for simple quad
    static GLuint quad_vao;

    // VBO object for a simple quad
    static GLuint quad_vbo;

public:
	// setup for any rendering util functions, should only be run once at initialization of program
	static void init();

	// Wrapper function to catch GL errors
	static void testGLError( const char *loc = "default" );

	// Draw a quad that takes up viewport
	static void drawQuad();
};

// The VBO for rendering a quad over the whole viewport
const GLfloat quad_vbo_data[] = {
//   x     y     w       u     v
    -1.0f,-1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,-1.0f, 0.0f,   1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     1.0f,-1.0f, 0.0f,   1.0f, 0.0f,
     1.0f, 1.0f, 0.0f,   1.0f, 1.0f
};

#endif