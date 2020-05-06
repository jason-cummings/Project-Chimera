#include "Shader.hpp"

#include <iostream>
#include "../../Utilities/Asset.hpp"

Shader::Shader() {
    program = glCreateProgram();
}

Shader::Shader( const char *s_name, const char *vs, const char *fs ) {
    shader_name = s_name;
    program = glCreateProgram();
    addShader( vs, GL_VERTEX_SHADER );
    addShader( fs, GL_FRAGMENT_SHADER );
    geom_prog = -1;
    link();
    if( prog_err ) {
        std::cerr << "Could not create shader program with files " << vs << " and " << fs << std::endl;
    }
}

Shader::Shader( const char *s_name, const char *vs, const char *gs, const char *fs ) {
    shader_name = s_name;
    program = glCreateProgram();
    addShader( vs, GL_VERTEX_SHADER );
    addShader( gs, GL_GEOMETRY_SHADER );
    addShader( fs, GL_FRAGMENT_SHADER );
    link();
    if( prog_err ) {
        std::cerr << "Could not create shader program with files " << vs << " and " << fs << std::endl;
    }
}

Shader::~Shader() {
    glDetachShader( program, vert_prog );
    glDeleteShader( vert_prog );

    if( geom_prog != -1 ) {
        glDetachShader( program, geom_prog );
        glDeleteShader( geom_prog );
    }

    glDetachShader( program, frag_prog );
    glDeleteShader( frag_prog );

    glDeleteProgram( program );
}

bool Shader::getProgErr() const { return prog_err; }


// Add a shader to the program
// shader_type is one of GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
void Shader::addShader( const char *fname, GLuint shader_type ) {
    // Get the shader source code
    std::string shader_path = "Shaders/" + std::string(fname);
    Asset shader_asset( shader_path );
    char *shader_src = shader_asset.getBuffer();
    if( shader_src == nullptr ) {
        // Could not read the shader asset
        prog_err = true;
        return;
    }

    // Create a shader program with the source code and compile it
    GLuint sh_prog = glCreateShader( shader_type );
    glShaderSource( sh_prog, 1, &shader_src, nullptr );
    glCompileShader( sh_prog );

    // Check if compilation was successful
    GLint comp_success;
    glGetShaderiv( sh_prog, GL_COMPILE_STATUS, &comp_success );
    if( comp_success == GL_FALSE ) {
        // Compilation failed, get the info and print it to stderr
        char shader_err[1024];
        int err_len;
        glGetShaderInfoLog( sh_prog, 1024, &err_len, shader_err );
        std::cerr << "Error compiling shader " << fname << ": " << shader_err << std::endl;
        prog_err = true;
        return;
    }

    // Attach the shader to the program and flag it to be deleted when the program is deleted
    glAttachShader( program, sh_prog );
    glDeleteShader( sh_prog );

    // Record the shader program id
    // Might not actually be necessary
    if( shader_type == GL_VERTEX_SHADER ) {
        vert_prog = sh_prog;
    }
    else if( shader_type == GL_FRAGMENT_SHADER ) {
        frag_prog = sh_prog;
    }
    else {
        geom_prog = sh_prog;
    }
}

void Shader::link() {
    GLint status;
    
    // Attempt to link the program
    glLinkProgram( program );
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( status == GL_FALSE ) {
        prog_err = true;
        std::cerr << "Error linking program" << std::endl;
    }

    // Validate the program
    glValidateProgram( program );
    glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
    if( status == GL_FALSE ) {
        prog_err = true;
        std::cerr << "Could not validate program: " << glGetError() << std::endl;
    }
}

// Set this shader to be used
void Shader::bind() const {
    glUseProgram(program);
}

// Get and store a uniform location
void Shader::addUniform( const std::string &uniform_name ) {
    uniform_locations[uniform_name] = glGetUniformLocation(program, uniform_name.c_str());
}

// Return a uniform location
GLuint Shader::getUniformLocation( const std::string &uniform_name ) {
    return uniform_locations[uniform_name];
}

// Load a 4x4 matrix uniform to the shader
void Shader::setUniformMat4( const std::string &uniform_name, const glm::mat4 &mat ) {
    glUniformMatrix4fv( getUniformLocation( uniform_name ), 1, GL_FALSE, glm::value_ptr(mat) );
}

// Load a 3x3 matrix uniform to the shader
void Shader::setUniformMat3( const std::string &uniform_name, const glm::mat3 &mat ) {
    glUniformMatrix3fv( getUniformLocation( uniform_name ), 1, GL_FALSE, glm::value_ptr(mat) );
}

// Load an int to the shader
void Shader::setUniformInt( const std::string &uniform_name, const int &val ) {
    glUniform1i( getUniformLocation( uniform_name ), val );
}

// Load a float to the shader
void Shader::setUniformFloat( const std::string &uniform_name, const float &val ) {
    glUniform1f( getUniformLocation( uniform_name ), val );
}

// Load a vec2 to the shader
void Shader::setUniformVec2( const std::string &uniform_name, const glm::vec2 &vec ) {
    glUniform2fv( getUniformLocation( uniform_name ), 1, glm::value_ptr(vec) );
}

// Load a vec3 to the shader
void Shader::setUniformVec3( const std::string &uniform_name, const glm::vec3 &vec ) {
    glUniform3fv( getUniformLocation( uniform_name ), 1, glm::value_ptr(vec) );
}

// Load a vec4 to the shader
void Shader::setUniformVec4( const std::string &uniform_name, const glm::vec4 &vec ) {
    glUniform4fv( getUniformLocation( uniform_name ), 1, glm::value_ptr(vec) );
}

// Return the name of the shader program
std::string Shader::getName() {
    return shader_name;
}
