#include "FXAA.hpp"

FXAA::FXAA(GLuint position_texture_in, GLuint color_texture_in, Framebuffer * target) : PostProcess(target) {
	position_texture = position_texture_in;
	color_texture = color_texture_in;
	sm = ShaderManager::getShaderManager();
}

void FXAA::apply() {

	// draw shaded image to the screen with FXAA applied. light blurring is applied to edges detected based on depth

	bindTargetBuffer();

	Shader *FXAA_shader = sm->getShader("FXAA");
	FXAA_shader->bind();

	FXAA_shader->setUniformInt( "position_texture", 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, position_texture );

	FXAA_shader->setUniformInt( "color_texture", 1 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, color_texture );

	RenderUtils::drawQuad();

	glUseProgram(0);

}