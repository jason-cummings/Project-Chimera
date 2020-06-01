#version 330 core

in vec2 texCoords;

layout (location = 0) out vec4 FragColor;

uniform bool horizontal;
uniform sampler2D colorTexture;

const float weight[5] = float[5](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
vec2 texelSize = 1.0 / textureSize(colorTexture, 0);

void main() {
   	vec2 offset;
   	if( horizontal ) offset = vec2(texelSize.x, 0.0);
   	else offset = vec2(0.0, texelSize.y);

   	vec3 result = texture(colorTexture, texCoords).rgb * weight[0];
   	for(int i = 1; i < 5; i++)
   	{
   		result += texture(colorTexture, texCoords + offset * i).rgb * weight[i];
   		result += texture(colorTexture, texCoords - offset * i).rgb * weight[i];
   	}

	// Calculate an alpha to use for blending
	float a = max(max(result.x, result.y), result.z);
	FragColor = vec4(result, a);
}
