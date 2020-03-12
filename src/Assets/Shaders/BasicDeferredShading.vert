#version 330

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
layout(location = 5) in vec2 Texture;

uniform mat4 Model;
uniform mat3 NormalMatrix;
uniform mat4 View;
uniform mat4 Projection;

uniform float materialShininess;

out vec4 fragPos;
out vec4 fragCol;
out vec3 fragNormal;
out vec2 texCoords;
out float shininess;

void main() {
	vec3 worldspaceNormal = normalize(Normal * NormalMatrix);

	vec4 worldVertexPosition = Model * Vertex;
	fragPos = worldVertexPosition / worldVertexPosition.w;
	fragCol = Color;
	fragNormal = worldspaceNormal;
	texCoords = Texture;
	shininess = materialShininess;
	gl_Position = Projection * View * worldVertexPosition;
}
