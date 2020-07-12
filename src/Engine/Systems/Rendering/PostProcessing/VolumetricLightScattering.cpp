#include "VolumetricLightScattering.hpp"

VolumetricLightScattering::VolumetricLightScattering( Framebuffer * target) : PostProcess(target) {
	occlusion_texture = 0;
	sm = ShaderManager::getShaderManager();
}

void VolumetricLightScattering::apply() {
	Shader *vls_shader = sm->getShader("volumetricLightScattering");
	vls_shader->bind();

	// calculate and set sun screen space location
	vls_shader->setUniformVec2("sunScreenCoords",sun_screen_coords);

	// set occlusion texture
	vls_shader->setUniformInt( "frame", 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, occlusion_texture );

	RenderUtils::testGLError("VolumetricLightScattering");

	bindTargetBuffer();

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	RenderUtils::drawQuad();
	glDisable( GL_BLEND );

	glUseProgram(0);
}

void VolumetricLightScattering::setSunScreenCoords(glm::vec2 sun_loc) {
	sun_screen_coords = sun_loc;
}