#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 texCoords;

struct PointLight {
    vec3 location;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linearAttenuation;
    float quadraticAttenuation;
};

uniform vec3 cameraLoc;
uniform PointLight light;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseTexture;
// uniform sampler2D shadowTexture;

uniform bool shadeCartoon;

const int cartoonLevels = 6;
float scaleFactor = 1.0 / cartoonLevels;

void main() {
    // Get all configuration
    vec3 fragPos = texture( positionTexture, texCoords ).rgb;
    vec3 fragNorm = normalize( texture( normalTexture, texCoords ).rgb );
    vec4 fDifShin = texture( diffuseTexture, texCoords );
    vec3 fragDiffuse = fDifShin.rgb;
    float fragShininess = fDifShin.a;
    // vec3 fragShadow = texture( shadowTexture, texCoords ).rgb;
    vec3 lightDir = normalize( light.location - fragPos );

    // Calculate luminosity
    float luminosity = 0.02;
    
    float lightDist = length(light.location - fragPos);
    float attenuation = 1.0 / (1.0 + light.linearAttenuation * lightDist + light.quadraticAttenuation * (lightDist * lightDist)); 

    // Calculate diffuse weight
    float diffuseWeight = max( dot( fragNorm, lightDir ), 0.0 );
    if( shadeCartoon ) {
        diffuseWeight = ceil( diffuseWeight * cartoonLevels ) * scaleFactor;
    }

    // Calculate specular weight
    vec3 viewDir = fragPos - cameraLoc;
    vec3 halfDir = normalize( lightDir + viewDir );
    float specAngle = max( dot(halfDir, fragNorm), 0.0 );
    float specularWeight = diffuseWeight > 0.0 ? pow( specAngle, fragShininess ) : 0.0;

    // Calculate total attenuated light weight and multiply it by the diffuse color
    vec3 lightWeight = attenuation * (light.ambient + (light.diffuse * diffuseWeight + light.specular * specularWeight));
    FragColor = vec4(lightWeight * fragDiffuse, 1.0);

    // Add the specular highlights to the bright texture
    BrightColor = vec4(specularWeight * attenuation * fragDiffuse, 0.0);

    // FragColor = vec4(vec3(specAngle),1.0);
}