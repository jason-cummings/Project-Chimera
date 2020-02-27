#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

// #define GL_GLEXT_PROTOTYPES 1
// #include <SDL_opengl.h>
#include <iostream>

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.hpp"
#include "TEMP_Cube.hpp"

class RenderSystem {
private:
    GLuint BASE_VAO; // Vertex array object necessary for shiz

    // The shader manager for the render system
    ShaderManager *sm;

    // Window size variables
    int view_width, view_height;
    float aspect_ratio, fov = 55.f;

    // TEMPORARY for test rendering
    float TEMP_ph = 30, TEMP_th = 30;
    TempCube TEMP_cube;
    
    // Model, View, and Projection matrices for the program
    glm::mat4 model_mat, view_mat, proj_mat;

    // Use for testing to pinpoint OpenGL errors
	void testGLError( const char *loc = "default" );

public:
    RenderSystem( int width, int height );

    // Update render systems view variables
    void reshape( int new_width, int new_height );

    // Create the necessary matrices for rendering
    void createMatrices();

    void render( double dt );
};

#endif
