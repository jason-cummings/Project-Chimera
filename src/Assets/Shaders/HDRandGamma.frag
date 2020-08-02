#version 410 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D colorTexture;

uniform float gamma;
uniform float exposure;
uniform bool useExposure;

void main() {
    vec3 hdrColor = texture(colorTexture, texCoords).rgb;

    // exposure tone mapping
    vec3 mapped = useExposure ? vec3(1.0) - exp(-hdrColor * exposure) : hdrColor; // hdrColor / (1.0 + hdrColor)
        
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}
