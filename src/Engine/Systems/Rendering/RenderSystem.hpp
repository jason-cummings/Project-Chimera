#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderManager.hpp"
#include "Framebuffer.hpp"
#include "../../GameObject.hpp"
#include "Mesh.hpp"

class RenderSystem {
private:
    // Temporary VAO to render everything for now
    GLuint BASE_VAO;

    //VAO for simple quad
    GLuint quad_vao;

    // VBO object for a simple quad
    GLuint quad_vbo;

    // The deferred rendering framebuffer
    Framebuffer deferred_buffer;

    // Variables for the output sizes of the textures
    int texture_width, texture_height;

    // The shader manager for the render system
    ShaderManager *sm;

    // Window size variables
    int view_width, view_height;
    float aspect_ratio, fov = 55.f;

    // Model, View, and Projection matrices for the program
    glm::mat4 view_mat, proj_mat;

    // vectors for lists of different types of geometry. This will be used to optimize the rendering pipeline by reducing 
    // how often the shader is switched during rendering
    std::vector<GameObject*> meshList;


    /**
        Rendering Pipeline Setup
    **/

    // Use for testing to pinpoint OpenGL errors
	void testGLError( const char *loc = "default" );

    // Set up the framebuffers necessary for the rendering process
    void createFramebuffers();



    /**
        Rendering Pipeline Util functions
         - functions that are used multiple times throughout the rendering pipeline
    **/

    // Draw a quad with texture tex that takes up the whole viewport
    void drawTexture( GLuint tex );

    // Draw a quad that tatkes up viewport
    void drawQuad();

    // draws the meshList
    void drawMeshList(bool useMaterials, Shader * shader);



    /**
        Rendering pipeline
    **/

    // Set up render lists - iterate through the scenegraph and identify what needs to be drawn
    void populateRenderLists(GameObject * gameObject);

    // Create the necessary matrices for rendering
    void createMatrices();

    // Do the deferred rendering step
    void deferredRenderStep();

    // shadows

    // shading step
    void shadingStep();

    // bloom

    // volumetric light scattering

    




public:
    RenderSystem( int width, int height );

    // Update render systems view variables
    void reshape( int new_width, int new_height );

    // RENDER
    void render( double dt, GameObject * sceneGraph );
};

// The VBO for rendering a quad over the whole viewport
const GLfloat quad_vbo_data[] = {
//   x  y  w    u  v
    -1,-1, 0,   0, 0,
     1,-1, 0,   1, 0,
    -1, 1, 0,   0, 1,
	-1, 1, 0,   0, 1,
     1,-1, 0,   1, 0,
     1, 1, 0,   1, 1
};

#endif
