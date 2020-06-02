#version 460 core

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 diffuse;
layout (location = 3) out vec4 emissive;
layout (location = 4) out vec4 occlusion;



in vec3 texCoords;

uniform samplerCube skybox;

uniform vec3 toLight;
uniform float threshold;
// float threshold = .97f;

void main() {
	vec3 normalized_texCoords = normalize(texCoords);
	vec4 color = texture(skybox, normalized_texCoords);

	position = vec4(0.0);

	normal = vec4(0.0);

    diffuse.rgb = color.rgb;
	diffuse.a = 0.0;

	//change this if bloom is wanted on the skybox
	emissive = vec4(color.rgb,0.0);

	//might need changes:
	float light_dot_product = dot(normalized_texCoords,toLight);

	// set the light dot product to zero if under threshold
	light_dot_product = light_dot_product > threshold? (light_dot_product - threshold) / (1.0f - threshold):0.0f;

	occlusion = vec4(color.rgb * light_dot_product,1.0f);
	// occlusion = vec4(color.rgb / color.w, color.w);
}
