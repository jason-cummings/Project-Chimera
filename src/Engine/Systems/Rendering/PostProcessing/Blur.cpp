#include "Blur.hpp"

Blur::Blur(GLuint to_blur_texture_in, Framebuffer * target) : PostProcess(target) {
	to_blur_texture = to_blur_texture_in;
	sm = ShaderManager::getShaderManager();
    
    passes = 2;
    out_width = RenderUtils::getTextureWidth();
    out_height = RenderUtils::getTextureHeight();
}

void Blur::createFrameBuffers() {
	// Set up the pingpong buffers for blurring
	blur_buffer[0].addColorTexture( "FragColor", out_width, out_height );
	blur_buffer[1].addColorTexture( "FragColor", out_width, out_height );
} 

void Blur::apply() {
	Shader *blur_shader = sm->getShader("linear-blur");
	blur_shader->bind();
	blur_shader->setUniformInt( "colorTexture", 0 );
	glActiveTexture( GL_TEXTURE0 );

	current_blur_buffer = 0;

	for( int i=0; i<passes*2; i++ ) {
		blur_shader->bind();
		blur_buffer[current_blur_buffer].bind();
		glViewport( 0, 0, out_width, out_height );
		glClear( GL_COLOR_BUFFER_BIT );
		
		blur_shader->setUniformFloat( "horizontal", (float)current_blur_buffer );
		GLuint tex_to_use = i == 0 ? to_blur_texture : blur_buffer[!current_blur_buffer].getTexture( "FragColor" )->getID();
		glBindTexture( GL_TEXTURE_2D, tex_to_use );

		RenderUtils::drawQuad();

		current_blur_buffer = !current_blur_buffer;
	}

	glViewport( 0, 0, out_width, out_height );

	RenderUtils::testGLError("Blur");


	bindTargetBuffer();

	Shader *quad_shader = sm->getShader( "quad" );
	quad_shader->bind();
	quad_shader->setUniformInt( "colorTexture", 0 );

	// Bind the texture to render
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, blur_buffer[!current_blur_buffer].getTexture("FragColor")->getID() );

	RenderUtils::drawQuad();

	glUseProgram(0);
}