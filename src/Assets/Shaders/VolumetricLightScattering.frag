#version 330

in vec2 texCoords;

out vec4 FragColor;

uniform vec2 sunScreenCoords;

float Density = .9f;
float Weight = .06f;
float Decay = .92f;

uniform sampler2D frame;

const int NUM_SAMPLES = 100;

void main()
{
	vec2 deltaTexCoords = (texCoords - sunScreenCoords) * 1.0f / NUM_SAMPLES * Density;

	vec4 color = vec4(0.0);
	float illuminationDecay = 1.0f;
	vec2 currentTexCoords = texCoords;

	for(int i = 0; i < NUM_SAMPLES; i++)
	{
		currentTexCoords -= deltaTexCoords;
		color += texture(frame, currentTexCoords) * illuminationDecay * Weight;
		illuminationDecay *= Decay;
	}

	FragColor = color;


}