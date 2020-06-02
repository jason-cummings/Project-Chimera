#version 460 core

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec2 Texture;

out vec2 texCoords;

void main() {
    texCoords = Texture;
    gl_Position = vec4(Vertex, 1.0);
}
