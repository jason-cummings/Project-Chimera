#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

#include "FBTexture.hpp"

class Framebuffer {
  private:
    GLuint framebuffer_id;
    GLuint depth_buffer;
    std::map<std::string, FBTexture *> textures;
    FBTexture *depth_texture;
    std::vector<GLenum> color_attachments;

    void addColorTextureHelper(std::string texture_name, int width, int height, bool high_precision);

  public:
    Framebuffer();
    ~Framebuffer();

    // Set this framebuffer as the active framebuffer
    void bind();
    void addColorTexture(std::string texture_name, int width, int height);
    void addColorTextureHighPrecision(std::string texture_name, int width, int height);
    void addDepthBuffer(int width, int height);
    void addDepthTexture(std::string texture_name, int width, int height);

    // Clear all textures and internal buffers
    void clearAll();

    FBTexture *getTexture(std::string texture_name);
    inline GLuint getID() { return framebuffer_id; }
    inline FBTexture *getDepthTexture() { return depth_texture; }
};

#endif