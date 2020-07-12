#ifndef BLOOM_HPP
#define BLOOM_HPP

#include <GL/glew.h>
#include "PostProcess.hpp"
#include "./../ShaderManager.hpp"
#include "./../RenderUtils.hpp"

#define BLOOM_PASSES 4

class Bloom : public PostProcess {
private:
	ShaderManager *sm;
	GLuint bright_texture; // texture that represents bright parts of the scene

	bool current_blur_buffer;
    Framebuffer blur_buffer[2];

public:
	Bloom(Framebuffer * target);
	void createFrameBuffers() override;
	void clearFrameBufferTextures() override;

	inline void setBrightTexture(GLuint bright_texture_in) { bright_texture = bright_texture_in; }

	void apply() override;
};

#endif