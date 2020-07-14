#version 410 core

layout (location = 0) out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform vec3 lightLocation;
uniform mat4 lightView;
uniform mat4 lightProjection;

const float g_MinVariance = 0.00005;

float linstep( float min, float max, float v ) {
    return clamp((v - min) / (max - min), 0.0, 1.0);
}

float ReduceLightBleeding( float pMax, float amount ) {
    // Remove the [0, Amount] tail and linearly rescale (Amount, 1].
    return linstep( amount, 1.0, pMax );
}

vec3 chebyshevUpperBound( vec2 moments, float t ) {
    // One-tailed inequality valid if t > moments.x
    float p = float(t <= moments.x);
    
    // Compute Variance
    float variance = moments.y - (moments.x*moments.x);
    variance = max(variance, g_MinVariance);

    // Compute probabilistic upper bound.
    float d = t - moments.x;
    float pMax = variance / (variance + d*d);
    float clampedPMax = ReduceLightBleeding( pMax, 0.5 );

    return vec3( max(p, clampedPMax) );
}

vec2 computeMoments( float depth ) {
    vec2 moments;

    // First moment is the depth itself.
    moments.x = depth;
    
    // Compute partial derivatives of depth.
    float dx = dFdx( depth );
    float dy = dFdy( depth );

    // Compute second moment over the pixel extents.
    moments.y = depth*depth + 0.25*(dx*dx + dy*dy);

    return moments;    
} 

void main() {
    vec3 outCol = vec3(1.0);

	vec3 worldLoc = texture(positionTexture, texCoords).rgb;
	vec3 worldNorm = normalize( texture(normalTexture, texCoords).rgb );
	vec3 lightDir = -normalize( lightLocation );

    vec4 lightSpaceLoc = lightProjection * lightView * vec4( worldLoc, 1.0 );
    vec3 projLoc = lightSpaceLoc.xyz * 0.5 + 0.5;

    // Test if the fragment is on the depth texture
    if( projLoc.x >= 0.0 && projLoc.x <= 1.0 && projLoc.y >= 0.0 && projLoc.y <= 1.0 ) {
        // Get depth of the fragment from the light
        float depth = texture(depthTexture, projLoc.xy).r;

        // vec2 moments = vec2( depth, depth*depth ); // naive
        vec2 moments = computeMoments( depth ); // advanced

        // Get the depth of the fragment in consideration
        float fragDepth = projLoc.z;

        // Run the VSM test
        outCol = chebyshevUpperBound( moments, fragDepth );
    }

    FragColor = vec4(outCol, 1.0);
}
