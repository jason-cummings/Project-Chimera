#ifndef FXAA_HPP
#define FXAA_HPP

#include <GL/glew.h>
#include "PostProcess.hpp"
#include "./../ShaderManager.hpp"
#include "./../RenderUtils.hpp"

class FXAA : public PostProcess {
private:
	ShaderManager *sm;
	GLuint position_texture; // depth texture to only blur edges and avoid blurring a mesh's texture
	GLuint color_texture; // the texture that will be blurred
public:
	FXAA(GLuint position_texture_in, GLuint color_texture_in, Framebuffer * target);

	void apply() override;
};

#endif