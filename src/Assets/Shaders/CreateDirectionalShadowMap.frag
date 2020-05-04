#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 Secondary;
layout (location = 2) out vec4 Three;
layout (location = 3) out vec4 Four;

in vec2 texCoords;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform bool iterate;
uniform vec3 lightLocation;
uniform mat4 lightView;
uniform mat4 lightProjections[4];
uniform float lightDistanceThresholds[4];

uniform vec3 cameraLocation;

void main() {
    vec3 outCol = vec3(1.0);

	vec3 worldLoc = texture(positionTexture, texCoords).rgb;
	vec3 worldNorm = normalize( texture(normalTexture, texCoords).rgb );
	vec3 lightDir = -normalize( lightLocation );
    
    // Determine which portion of the shadow map to use
    int mapIndex = 2;
    if( iterate ) {
        mapIndex = 3;
        if( distance(worldLoc, cameraLocation) < lightDistanceThresholds[0] ) {
            mapIndex = 0;
        } 
        else if( distance(worldLoc, cameraLocation) < lightDistanceThresholds[1] ) {
            mapIndex = 1;
        } 
        else if( distance(worldLoc, cameraLocation) < lightDistanceThresholds[2] ) {
            mapIndex = 2;
        }
    }

    // Make sure that lighting can be applied to this fragment
    if( dot(lightDir, worldNorm) < 0.0 ) {
        // Get the location of the fragment in lightspace
        vec4 lightSpaceLoc = lightProjections[mapIndex] * lightView * vec4( worldLoc, 1.0 );
		vec3 projLoc = lightSpaceLoc.xyz * 0.5 + 0.5;

        // Test if the fragment is on the depth texture
        if( projLoc.x >= 0.0 && projLoc.x <= 1.0 && projLoc.y >= 0.0 && projLoc.y <= 1.0 ) {
            // Adjust the x texture coord to account for 4 stage texture
            projLoc.x = (projLoc.x + mapIndex)/4.0;

            // Get depth of the fragment from the light
            float closestDepth = texture(depthTexture, projLoc.xy).r;

            // Get the depth of the fragment in consideration
            float fragDepth = projLoc.z;

            float temp = closestDepth * 10.0 - 5.0;
            Secondary = vec4(temp, temp, temp, 1.0);
            Three = vec4(projLoc.xy, 0.0, 1.0);
        
            // Test if the fragment minus a bias is closer than the current depth
            float bias = 0.0002*(mapIndex+1);
            if( fragDepth - bias > closestDepth ) {
                // Fragment should be lit
                outCol = vec3(0.0);
            }
        }
    }
    else {
        outCol = vec3(0.0);
    }

    FragColor = vec4(outCol, 1.0);
    Four = vec4(vec3(texture(depthTexture, texCoords).r * 10.0 - 5.0), 1.0);
}
