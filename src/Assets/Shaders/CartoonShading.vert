#version 330

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec2 UV;

out vec2 texCoords;

void main() {
	texCoords = UV;
	gl_Position = vec4(Vertex, 1.0);
}