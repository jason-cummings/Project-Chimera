#ifndef VOLUMETRIC_LIGHT_SCATTERING_HPP
#define VOLUMETRIC_LIGHT_SCATTERING_HPP

#include <GL/glew.h>
#include "PostProcess.hpp"
#include "./../ShaderManager.hpp"
#include "./../RenderUtils.hpp"

class VolumetricLightScattering : public PostProcess {
private:
	ShaderManager *sm;
	GLuint occlusion_texture; // occlusion texture that keeps track of how the light is being blocked

	glm::vec2 sun_screen_coords;
public:
	VolumetricLightScattering(GLuint occlusion_texture_in, Framebuffer * target);

	void apply() override;

	void setSunScreenCoords(glm::vec2 sun_loc);
};

#endif