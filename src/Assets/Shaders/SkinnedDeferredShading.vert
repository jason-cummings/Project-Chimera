#version 330

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
layout(location = 5) in vec2 Texture;

layout(location = 6) in int Joint_Index_1;
layout(location = 7) in float Joint_Weight_1;

layout(location = 8) in int Joint_Index_2;
layout(location = 9) in float Joint_Weight_2;

layout(location = 10) in int Joint_Index_3;
layout(location = 11) in float Joint_Weight_3;

layout(location = 12) in int Joint_Index_4;
layout(location = 13) in float Joint_Weight_4;

uniform mat4 Model;
uniform mat3 NormalMatrix;
uniform mat4 View;
uniform mat4 Projection;

uniform float materialShininess;

uniform mat4 boneMatrices[100];

out vec4 fragPos;
out vec4 fragCol;
out vec3 fragNormal;
out vec2 texCoords;
out float shininess;

void main() {
	vec3 worldspaceNormal = normalize( NormalMatrix * Normal);

	mat4 animatedVertexPosition = (boneMatrices[Joint_Index_1] * Joint_Weight_1 + 
								   boneMatrices[Joint_Index_2] * Joint_Weight_2 + 
								   boneMatrices[Joint_Index_3] * Joint_Weight_3 + 
								   boneMatrices[Joint_Index_4] * Joint_Weight_4);

	vec4 worldVertexPosition = Model * animatedVertexPosition * Vertex ;
	fragPos = worldVertexPosition / worldVertexPosition.w;
	fragCol = Color;
	fragNormal = worldspaceNormal;
	texCoords = Texture;
	shininess = materialShininess;
	gl_Position = Projection * View * worldVertexPosition;
}
