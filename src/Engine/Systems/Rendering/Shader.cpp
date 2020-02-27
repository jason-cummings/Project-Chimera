#include "Shader.hpp"

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

// Read the shader from file fname (will append path to Shaders directory)
// shader_src will be allocated and assigned the contents of the file
// Returns true iff the file's contents were successfully copied to *shader_src, else false
bool Shader::readShaderSource( const char *fname, char **shader_src ) {
    // Get the full path to the shader
    const char* fullfname = (std::string("./Assets/Shaders/") + std::string(fname)).c_str();

    // Attempt to open the file for reading
    FILE* file = std::fopen( fullfname, "r" );
    if( file == nullptr ) {
        // Couldn't open the file
		std::cerr << "Could not open file: " << fname << std::endl;
	    prog_err = true;
        return false;
	}

    // Get the number of bytes in the sile and allocate a char buffer
	std::fseek( file, 0, SEEK_END );
	size_t nbytes = std::ftell( file );
	std::fseek( file, 0, SEEK_SET );
    if( (*shader_src = (char *)calloc(nbytes+1, sizeof(char))) == NULL ) {
        // Couldn't allocate the buffer
        std::cerr << "Unable to allocate " << nbytes+1 << " bytes for file " << fname << std::endl;
        prog_err = true;
        return false;
    }

    // Attempt to read in the file to shader_src
    if( std::fread( *shader_src, nbytes, 1, file ) != 1 ) {
        std::cerr << "Error reading file " << fname << std::endl;
        prog_err = true;
        free(shader_src);
        return false;
    }

    // Done, close the file
    std::fclose( file );
    return true;
}

// Add a shader to the program
// shader_type is one of GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
void Shader::addShader( const char *fname, GLuint shader_type ) {
    // Get the shader source code
    char *shader_src;
    if( !readShaderSource( fname, &shader_src ) ) {
        // Could not get the shader source code
        return;
    }

    // Create a shader program with the source code and compile it
    GLuint sh_prog = glCreateShader( shader_type );
    glShaderSource( sh_prog, 1, &shader_src, nullptr );
    glCompileShader( sh_prog );
    free( shader_src ); // Done with source code, free it

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
    GLuint err;
}

// Return a uniform location
GLuint Shader::getUniformLocation( const std::string &uniform_name ) {
    return uniform_locations[uniform_name];
}

// Load a 4x4 matrix uniform to the shader
void Shader::setUniformMat4( const std::string &uniform_name, const glm::mat4 &mat ) {
    bind();
    glUniformMatrix4fv( getUniformLocation( uniform_name ), 1, GL_FALSE, glm::value_ptr(mat) );
}

std::string Shader::getName() {
    return shader_name + std::string("   <- that ting");
}
