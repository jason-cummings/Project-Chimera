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

    static int texture_width;
    static int texture_height;

    static int view_width;
    static int view_height;

public:
	// setup for any rendering util functions, should only be run once at initialization of program
	static void init();

	// Wrapper function to catch GL errors
	static void testGLError( const char *loc = "default" );

	// Draw a quad that takes up viewport
	static void drawQuad();

	static int getTextureHeight() { return texture_height; }
	static int getTextureWidth() { return texture_width; }

	static int getViewHeight() { return view_height; }
	static int getViewWidth() { return view_width; }

	inline static void setTextureHeight( int texture_height_in ) { texture_height = texture_height_in; }
	inline static void setTextureWidth( int texture_width_in ) { texture_width = texture_width_in; }

	inline static void setViewHeight( int view_height_in ) { view_height = view_height_in; }
	inline static void setViewWidth( int view_width_in ) { view_width = view_width_in; }

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