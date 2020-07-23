#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 texCoords;

uniform sampler2D emissiveTexture;

void main() {
    vec4 emission = texture( emissiveTexture, texCoords );

    FragColor = vec4( emission.xyz, 0.0 );
    BrightColor = emission * emission.a;
}
