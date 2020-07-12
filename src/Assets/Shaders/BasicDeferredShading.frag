#version 330 core

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 diffuse;
layout (location = 3) out vec4 emissive;
layout (location = 4) out vec4 occlusion;


in vec4 fragPos;
in vec4 fragCol;
in vec3 fragNormal;
in mat3 toWorld;
in vec2 texCoords;
in float shininess;

uniform sampler2D colorTexture;
uniform sampler2D emissiveTexture;

void main() {
	position = fragPos;
	normal = vec4(fragNormal, 1.0);
    diffuse.rgb = texture(colorTexture, texCoords).rgb;
	diffuse.a = shininess;
	emissive = vec4(texture(emissiveTexture, texCoords).xyz,1.0);
	occlusion = vec4(vec3(0.0), 1.0);
}
