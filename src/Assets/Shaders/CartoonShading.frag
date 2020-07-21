#version 410

uniform vec3 cameraLoc;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D emissiveTexture;
uniform sampler2D shadowTexture;

in vec2 texCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Light {
	vec3 location;
	vec3 diffuse;
	vec3 specular;
	float linearAttenuation;
	float quadraticAttenuation;
	bool directional;
};

uniform Light light;

uniform float ambientAmount;
uniform bool applyShadows;

float tempGamma = 2.2;

int levels = 4;
float scaleFactor = 1.0 / levels;


void main()
{
	vec3 fragPos = texture(positionTexture, texCoords).rgb;
	vec3 normal = normalize(texture(normalTexture, texCoords).rgb);
	vec3 diffuseColor = texture(diffuseTexture, texCoords).rgb;
	float shininess = texture(diffuseTexture, texCoords).a + .1f;
	vec4 emission = texture(emissiveTexture, texCoords);
	vec3 shadow = vec3(1.0); if( applyShadows ) shadow = texture(shadowTexture, texCoords).rgb;
	// vec4 reflectivity = texture(reflectivityTexture, texCoords);

	vec3 finalColor = diffuseColor * ambientAmount; // replace diffuseColor with ambient color

	float luminosity = 0.02;

	vec3 viewDirection = normalize(cameraLoc - fragPos);

	BrightColor = vec4(0.0);

	// lighting
	

	vec3 lightDirection = light.directional ? normalize(light.location) : normalize(light.location - fragPos);
	
	// diffuse

	float diffuseAmount = max(dot(normalize(normal), lightDirection),0.0);

	// give the diffuse lighting a cartoon effect by clamping the color values at different levels
	// diffuseAmount = floor(diffuseAmount * levels) * scaleFactor;
	
	
	// Specular

	vec3 R = reflect( -1.0 * lightDirection,normal);
	vec3 halfwaydir = normalize(lightDirection + viewDirection);
	float specularAmount = pow(max(dot(normal,halfwaydir),0.0),shininess);
	
	vec3 specular = vec3(0.0);
	// if(diffuseAmount > 0.0)
	// 	specular = specularAmount * light.specular * diffuseColor;

	// calculate attenuation of the light

	float d = length(light.location - fragPos);
	float attenuation = light.directional ? 1.0 : 1.0 / (1.0 + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

	

	// combine shadow, diffuse color, and specular color and apply attenuation

	finalColor += shadow * (diffuseAmount * diffuseColor * light.diffuse + specular) * attenuation;

	// luminosity += (diffuseAmount + specularAmount) * attenuation * shadowVal;

	BrightColor += vec4(shadow * (specular * attenuation), 0.0);


	//end lighting



	// vec3 R = reflect(-viewDirection,normal);
	// vec4 reflectionColor = texture(environmentMap, R);
	// finalColor = mix(finalColor , reflectionColor.xyz * luminosity * reflectivity.xyz + reflectionColor.xyz * reflectionColor.w * reflectivity.xyz, (1.0 - reflectivity.w));

	finalColor += emission.xyz;
	FragColor = vec4(finalColor, luminosity + (emission.x + emission.y + emission.z));
	BrightColor += emission * emission.a;

}
