#version 410 core

layout(location = 0) out vec4 FragColor;

in vec2 texCoords;

uniform vec4 textColor;
uniform sampler2D textTexture;

void main() {    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, texCoords).r);
    FragColor = textColor * sampled;
}
