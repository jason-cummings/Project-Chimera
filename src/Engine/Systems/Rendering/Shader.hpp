#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Utilities/Asset.hpp"

enum ShaderAttrib {
    Vertex = 0,
    Color = 1,
    Normal = 2,
    Tangent = 3,
    Bitangent = 4,
    Texture = 5,
    // Joint_Index_1 = 6,
    // Joint_Weight_1 = 7,
    // Joint_Index_2 = 8,
    // Joint_Weight_2 = 9,
    // Joint_Index_3 = 10,
    // Joint_Weight_3 = 11,
    // Joint_Index_4 = 12,
    // Joint_Weight_4 = 13
    Joint1to4 = 6,
    Joint5to8 = 7,
    Joint9to12 = 8,
    Joint13to16 = 9,
    Joint17to20 = 10,
    Joint21to22 = 11
};

enum ShaderAttrib2D {
    Vertex2D = 0,
    Texture2D = 1
};

enum ShaderAttribOverlay {
    VertexOverlay = 0,
    TextureOverlay = 1
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
    
    // Link the program
    void link(); 

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
    // <shader>.bind() must be called before any of these are called
    void setUniformMat4( const std::string &uniform_name, const glm::mat4 &mat );
    void setUniformMat3( const std::string &uniform_name, const glm::mat3 &mat );
    void setUniformInt( const std::string &uniform_name, const int &val );
    void setUniformFloat( const std::string &uniform_name, const float &val );
    void setUniformVec2( const std::string &uniform_name, const glm::vec2 &vec );
    void setUniformVec3( const std::string &uniform_name, const glm::vec3 &vec );
    void setUniformVec4( const std::string &uniform_name, const glm::vec4 &vec );
};

#endif
