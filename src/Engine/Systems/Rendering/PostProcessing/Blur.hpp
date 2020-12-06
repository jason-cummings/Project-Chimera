#ifndef BLUR_HPP
#define BLUR_HPP

#include "./../RenderUtils.hpp"
#include "./../ShaderManager.hpp"
#include "PostProcess.hpp"
#include <GL/glew.h>

class Blur : public PostProcess {
  private:
    ShaderManager *sm;
    GLuint to_blur_texture;

    bool current_blur_buffer;
    Framebuffer blur_buffer[2];

    int passes;
    int out_width, out_height;

  public:
    Blur(GLuint to_blur_texture_in, Framebuffer *target);

    void createFrameBuffers() override;

    inline void setOutSize(int width, int height) {
        out_width = width;
        out_height = height;
    }
    inline void setBlurPasses(int passes_in) { passes = passes_in; }

    void apply() override;
};

#endif