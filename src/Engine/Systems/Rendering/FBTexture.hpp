#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class FBTexture {
private:
    GLuint id;
    std::string name;

public:
    FBTexture( GLuint id_, std::string name_ );

    // Bind this texture to the specified texture unit
    void bind( GLenum texture_unit );
};

#endif