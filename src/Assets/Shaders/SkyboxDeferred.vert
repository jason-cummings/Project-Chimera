#version 410

layout(location = 0) in vec3 Vertex;


uniform mat4 View;
uniform mat4 Projection;

out vec3 texCoords;

void main() {
	// texCoords are for cubemap
	texCoords = Vertex;
	gl_Position = Projection * View * vec4(2.0f * Vertex,1.0);
}
