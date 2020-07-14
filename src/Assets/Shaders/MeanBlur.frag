#version 410 core

in vec2 texCoords;

layout (location = 0) out vec4 FragColor;

uniform bool horizontal;
uniform sampler2D colorTexture;

vec2 texelSize = 1.0 / textureSize(colorTexture, 0);
// float texOffset[5] = { 0, 1.5, 3.5, 5.5, 7.5 };

void main() {
   	vec2 offset;
   	if( horizontal ) offset = vec2(texelSize.x, 0.0);
   	else offset = vec2(0.0, texelSize.y);

   	vec3 result = texture(colorTexture, texCoords).rgb;
   	for(int i = 1; i < 5; i++)
   	{
   		result += texture(colorTexture, texCoords + offset * i).rgb;
   		result += texture(colorTexture, texCoords - offset * i).rgb;
   	}
    
    result = result / 9.0;

	// Calculate an alpha to use for blending
	float a = max(max(result.x, result.y), result.z);
	FragColor = vec4(result, a);
}
