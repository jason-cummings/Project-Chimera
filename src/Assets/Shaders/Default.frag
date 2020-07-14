#version 410 core

in vec4 col;
// in vec3 norm;
// in vec3 tan;
// in vec3 bitan;
// in vec2 tex;

out vec4 FragColor;

// uniform vec3 specularMaterialColor;

// uniform sampler2D colorTexture;

void main() {
	// FragColor = vec4((.1 * col).xyz,0.0) + texture(colorTexture, tex);
    FragColor = col;
}
