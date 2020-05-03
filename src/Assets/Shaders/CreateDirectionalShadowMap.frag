#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform mat4 lightView;
uniform mat4 lightProjection;
uniform vec3 lightLocation;

void main() {
    vec3 outCol = vec3(0.0);

	vec3 worldLoc = texture(positionTexture, texCoords).rgb;
	vec3 worldNorm = normalize( texture(normalTexture, texCoords).rgb );
	vec3 lightDir = -normalize( lightLocation );

    // Make sure that lighting can be applied to this fragment
    if( dot(lightDir, worldNorm) < 0.0 ) {
        // Get the location of the fragment in lightspace
        vec4 lightSpaceLoc = lightProjection * lightView * vec4( worldLoc, 1.0 );
		vec3 projLoc = lightSpaceLoc.xyz * 0.5 + 0.5;

        // Test if the fragment is on the depth texture
        if( projLoc.x >= 0.0 && projLoc.x <= 1.0 && projLoc.y >= 0.0 && projLoc.y <= 1.0 ) {
            // Get depth of the fragment from the light
            float closestDepth = texture(depthTexture, projLoc.xy).r;

            // Get the depth of the fragment in consideration
            float fragDepth = projLoc.z;

            // FragColor = vec4(closestDepth, closestDepth, closestDepth, 1.0);
        
            // Test if the fragment minus a bias is closer than the current depth
            float bias = 0.001;
            if( fragDepth - bias < closestDepth ) {
                // Fragment should be lit
                outCol = vec3(1.0);
            }
        }
    }

    FragColor = vec4(outCol, 1.0);
}
