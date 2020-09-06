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

	static GLuint sphere_vao;
	static GLuint sphere_vbo;
	static int n_sphere_verts;

	// The rendering resolution for the game
    static int texture_width;
    static int texture_height;

	// The viewport size to render to
    static int view_width;
    static int view_height;

public:
	// setup for any rendering util functions, should only be run once at initialization of program
	static void init();

	// Wrapper function to catch GL errors
	static void testGLError( const char *loc = "default" );

	// Draw a quad that takes up viewport
	static void drawQuad();

	// Draw a low poly sphere
	static void drawSphere();

	inline static int getTextureHeight() { return texture_height; }
	inline static int getTextureWidth() { return texture_width; }

	inline static int getViewHeight() { return view_height; }
	inline static int getViewWidth() { return view_width; }

	inline static void setTextureHeight( int texture_height_in ) { texture_height = texture_height_in; }
	inline static void setTextureWidth( int texture_width_in ) { texture_width = texture_width_in; }

	inline static void setViewHeight( int view_height_in ) { view_height = view_height_in; }
	inline static void setViewWidth( int view_width_in ) { view_width = view_width_in; }
};

#endif