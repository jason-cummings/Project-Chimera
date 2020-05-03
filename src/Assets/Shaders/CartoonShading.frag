#version 330

uniform vec3 cameraLoc;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D emissiveTexture;
//uniform usampler2D shadowTexture;

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

float tempGamma = 2.2;

int levels = 4;
float scaleFactor = 1.0 / levels;


void main()
{
	vec3 fragPos = texture(positionTexture, texCoords).rgb;
	vec3 normal = normalize(texture(normalTexture, texCoords).rgb);
	vec3 diffuseColor = texture(diffuseTexture, texCoords).rgb;
	float shininess = texture(diffuseTexture, texCoords).a;
	vec4 emission = texture(emissiveTexture, texCoords);
	//vec3 shadow = texture(shadowTexture, texCoords).rgb;
	// vec4 reflectivity = texture(reflectivityTexture, texCoords);

	vec3 finalColor = diffuseColor * ambientAmount; // replace diffuseColor with ambient color

	float luminosity = 0.02;

	vec3 viewDirection = normalize(cameraLoc - fragPos);

	BrightColor = vec4(0.0);

	// lighting

	float shadowVal = 1.0;//shadow.r;
	

	// vec3 lightDirection = light.directional ? normalize(light.location) : normalize(light.location - fragPos);
	vec3 lightDirection = normalize(light.location - fragPos);
	float diffuseAmount = max(dot(normalize(normal), lightDirection),0.0);
	
	
	//vec3 diffuse = diffuseColor * diffuseAmount * light.diffuse;

	vec3 R = reflect( -1.0 * lightDirection,normal);
	//vec3 halfwaydir = normalize(lightDirection + viewDirection);

	//float specularAmount = max(dot(normal,halfwaydir),0.0);
	float specularAmount = pow(max(dot(R,viewDirection),0.0),shininess);

	if(specularAmount > .4) {
		specularAmount = .4;
	}
	else specularAmount = 0.0f;
	

	vec3 specular = vec3(0.0);
	if(diffuseAmount > 0.0)
		specular = specularAmount * light.specular;


	float d = length(light.location - fragPos);
	float attenuation = 1.0 / (1.0 + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

	//finalColor += shadowVal * (diffuse * attenuation + specular * attenuation);

	//finalColor += (diffuseAmount * light.diffuse + specular) * attenuation;// * diffuseColor;

	diffuseAmount = floor(diffuseAmount * levels) * scaleFactor;

	finalColor += (diffuseAmount * light.diffuse + specular) * attenuation * diffuseColor;

	luminosity += (diffuseAmount + specularAmount) * attenuation * shadowVal;

	BrightColor += vec4(shadowVal * (specular * attenuation),0.0);


	//end lighting



	// vec3 R = reflect(-viewDirection,normal);
	// vec4 reflectionColor = texture(environmentMap, R);
	// finalColor = mix(finalColor , reflectionColor.xyz * luminosity * reflectivity.xyz + reflectionColor.xyz * reflectionColor.w * reflectivity.xyz, (1.0 - reflectivity.w));

	finalColor += emission.xyz;
	FragColor = vec4(finalColor, luminosity + (emission.x + emission.y + emission.z));
	BrightColor += emission * emission.a;
	
}
