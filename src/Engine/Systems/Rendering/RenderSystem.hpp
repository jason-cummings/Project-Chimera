#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <iostream>
#include <SDL_opengl.h>

class RenderSystem {
private:
    // Window size variables
    int view_width, view_height;
    float aspect_ratio;

    // Use for testing to pinpoint OpenGL errors
	void testGLError( const char *loc = "default" );
public:
    RenderSystem( int width, int height );

    // Update render systems view variables
    void reshape( int new_width, int new_height );

    void render( double dt );
};

#endif
