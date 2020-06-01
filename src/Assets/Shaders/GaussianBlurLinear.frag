#version 330 core

in vec2 texCoords;

layout (location = 0) out vec4 FragColor;

uniform bool horizontal;
uniform sampler2D colorTexture;

vec2 texelSize = 1.0 / textureSize(colorTexture, 0);
const float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
const float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

void main() {
   	vec2 sample_offset = horizontal ? vec2(texelSize.x, 0.0) : vec2(0.0, texelSize.y);

   	vec3 result = texture(colorTexture, texCoords).rgb * weight[0];
   	for(int i = 1; i < 3; i++)
   	{
   		result += texture(colorTexture, texCoords + sample_offset * offset[i]).rgb * weight[i];
   		result += texture(colorTexture, texCoords - sample_offset * offset[i]).rgb * weight[i];
   	}

	// Calculate an alpha to use for blending
	float a = max(max(result.x, result.y), result.z);
	FragColor = vec4(result, a);
}
