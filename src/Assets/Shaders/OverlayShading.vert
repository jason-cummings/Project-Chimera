#version 410 core

layout(location = 0) in vec2 Vertex;
layout(location = 1) in vec2 Texture;

out vec2 texCoords;

uniform mat4 Model;
uniform mat4 Projection;

void main() {
    texCoords = Texture;
    gl_Position = Projection * Model * vec4(Vertex, 0.0, 1.0);
}
