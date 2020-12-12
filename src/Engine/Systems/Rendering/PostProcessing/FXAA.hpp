#ifndef FXAA_HPP
#define FXAA_HPP

#include "./../RenderUtils.hpp"
#include "./../ShaderManager.hpp"
#include "PostProcess.hpp"
#include <GL/glew.h>

class FXAA : public PostProcess {
  private:
    ShaderManager *sm;
    GLuint position_texture; // depth texture to only blur edges and avoid blurring a mesh's texture
    GLuint color_texture;    // the texture that will be blurred
  public:
    FXAA(Framebuffer *target);

    inline void setPositionTexture(GLuint position_texture_in) { position_texture = position_texture_in; }
    inline void setColorTexture(GLuint color_texture_in) { color_texture = color_texture_in; }

    void apply() override;
};

#endif