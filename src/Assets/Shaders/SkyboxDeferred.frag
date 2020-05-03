#version 330 core

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 diffuse;
layout (location = 3) out vec4 emissive;
layout (location = 4) out vec4 occlusion;



in vec3 texCoords;

uniform samplerCube skybox;

void main() {
	vec4 color = texture(skybox, texCoords);

	position = vec4(0.0);

	normal = vec4(0.0);

    diffuse.rgb = color.rgb;
	diffuse.a = 0.0;

	//change this if bloom is wanted on the skybox
	emissive = vec4(color.rgb,0.0);

	//might need changes:
	occlusion = vec4(color.rgb / color.w, color.w);
}
