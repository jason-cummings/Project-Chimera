#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "FBTexture.hpp"

class Framebuffer {
private:
    GLuint framebuffer_id;
    GLuint depth_buffer;
    std::map<std::string, FBTexture*> textures;
    std::vector<GLenum> color_attachments;

public:
    Framebuffer();
    ~Framebuffer();

    // Set this framebuffer as the active framebuffer
    void bind();
    void addColorTexture( std::string texture_name, int width, int height );
    void addDepthBuffer( int width, int height );

    FBTexture * getTexture( std::string texture_name );
};

// Special framebuffer that only renders depth
class ShadowFramebuffer {
private:
    GLuint framebuffer_id;
    GLuint depth_buffer;
    FBTexture *depth_texture;

public:
    ShadowFramebuffer();
    ~ShadowFramebuffer();

    // Set this framebuffer as the active framebuffer
    void bind();
    void addDepthBuffer( int width, int height );
    void addDepthTexture( std::string texture_name, int width, int height );

    inline FBTexture * getDepthTexture() { return depth_texture; }
};

#endif