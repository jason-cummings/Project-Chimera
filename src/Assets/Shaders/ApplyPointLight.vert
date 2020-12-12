#version 410 core

layout(location = 0) in vec4 Vertex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out float effectiveRadius;

void main() {
	vec4 pos = Projection * View * Model * Vertex;
	effectiveRadius = Model[0][0];
	gl_Position = pos;
}
