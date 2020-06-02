#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec2 texCoords;

uniform sampler2D colorTexture;
uniform sampler2D emissiveTexture;

void main() {
    FragColor = texture(colorTexture, texCoords);
	BrightColor = vec4(texture(emissiveTexture, texCoords).rgb, FragColor.a);
}
