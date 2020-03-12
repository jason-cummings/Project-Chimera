#version 330 core

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;
// layout(location = 2) in vec3 Normal;
// layout(location = 3) in vec3 Tangent;
// layout(location = 4) in vec3 Bitangent;
// layout(location = 5) in vec2 Texture;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 col;
// out vec3 norm;
// out vec3 tan;
// out vec3 bitan;
// out vec2 tex;

void main() {
	col = Color;
	// norm = Normal;
	// tan = Tangent;
	// bitan = Bitangent;
	// tex = Texture;
	gl_Position = Projection * View * Model * Vertex;
}
