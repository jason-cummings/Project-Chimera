#ifndef SHADER_H
#define SHADER_H

// #define GL_GLEXT_PROTOTYPES 1
// #include <SDL_opengl.h>

#include <GL/glew.h>

#include <map>
#include <string>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Asset.hpp"

enum ShaderAttrib {
    Vertex = 0,
    Color = 1,
    Normal = 2,
    Tangent = 3,
    Bitangent = 4,
    Texture = 5
};

class Shader {
private:
    GLuint program;
    GLuint vert_prog;
    GLuint geom_prog;
    GLuint frag_prog;

    bool prog_err = false;

    std::string shader_name;
    std::map<std::string, GLuint> uniform_locations;

    // Add a shader to the program
    void addShader( const char *fname, GLuint shader_type ); 
    void link(); // Link the program

public:
    // Default - shouldn't be needed
    Shader();

    // Create vertex/fragment shader program
    Shader( const char *s_name, const char *vs, const char *fs );

    // Create vertex/geometry/fragment shader program
    Shader( const char *s_name, const char *vs, const char *gs, const char *fs );

    ~Shader();

    // Return whether or not there was an error
    bool getProgErr() const;
    std::string getName();

    // Set this shader to be used
    void bind() const;

    // Get and store a uniform location
    void addUniform( const std::string &uniform_name );

    // Return a uniform location
    GLuint getUniformLocation( const std::string &uniform_name );

    // Functions to set uniform values
    void setUniformMat4( const std::string &uniform_name, const glm::mat4 &mat );
};

#endif
