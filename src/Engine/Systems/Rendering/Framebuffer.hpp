#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <string>
#include <map>

#include "FBTexture.hpp"

class Framebuffer {
private:
    GLuint framebufferID;
    std::map<std::string, FBTexture*> textures;
    std::map<std::string, GLenum> colorAttachments;

public:
    Framebuffer();
    ~Framebuffer();

    // Set this framebuffer as the active framebuffer
    void bind();
    FBTexture * addColorTexture( std::string texture_name, int width, int height );
};

#endif