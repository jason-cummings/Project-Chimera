#version 410

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
layout(location = 5) in vec2 Texture;

// layout(location = 6) in int Joint_Index_1;
// layout(location = 7) in float Joint_Weight_1;

// layout(location = 8) in int Joint_Index_2;
// layout(location = 9) in float Joint_Weight_2;

// layout(location = 10) in int Joint_Index_3;
// layout(location = 11) in float Joint_Weight_3;

// layout(location = 12) in int Joint_Index_4;
// layout(location = 13) in float Joint_Weight_4;

layout(location = 6) in vec4 Joint1to4;
layout(location = 7) in vec4 Joint5to8;
layout(location = 8) in vec4 Joint9to12;
layout(location = 9) in vec4 Joint13to16;
layout(location = 10) in vec4 Joint17to20;
layout(location = 11) in vec2 Joint21to22;

uniform mat4 Model;
uniform mat3 NormalMatrix;
uniform mat4 View;
uniform mat4 Projection;

uniform float materialShininess;

uniform mat4 boneMatrices[25];


out vec4 fragPos;
out vec4 fragCol;
out vec3 fragNormal;
out vec2 texCoords;
out float shininess;

int one = 1;
int two = 2;
int three = 3;
int four = 4;

void main() {
	vec3 worldspaceNormal = normalize( NormalMatrix * Normal);

	
	mat4 boneTransform = boneMatrices[0] * Joint1to4[0];
	boneTransform += boneMatrices[1] * Joint1to4[1];
	boneTransform += boneMatrices[2] * Joint1to4[2];
	boneTransform += boneMatrices[3] * Joint1to4[3];

	boneTransform += boneMatrices[4] * Joint5to8[0];
	boneTransform += boneMatrices[5] * Joint5to8[1];
	boneTransform += boneMatrices[6] * Joint5to8[2];
	boneTransform += boneMatrices[7] * Joint5to8[3];

	boneTransform += boneMatrices[8] * Joint9to12[0];
	boneTransform += boneMatrices[9] * Joint9to12[1];
	boneTransform += boneMatrices[10] * Joint9to12[2];
	boneTransform += boneMatrices[11] * Joint9to12[3];

	boneTransform += boneMatrices[12] * Joint13to16[0];
	boneTransform += boneMatrices[13] * Joint13to16[1];
	boneTransform += boneMatrices[14] * Joint13to16[2];
	boneTransform += boneMatrices[15] * Joint13to16[3];

	boneTransform += boneMatrices[16] * Joint17to20[0];
	boneTransform += boneMatrices[17] * Joint17to20[1];
	boneTransform += boneMatrices[18] * Joint17to20[2];
	boneTransform += boneMatrices[19] * Joint17to20[3];

	boneTransform += boneMatrices[20] * Joint21to22[0];
	boneTransform += boneMatrices[21] * Joint21to22[1];
	


	vec4 animatedVertexPosition = boneTransform * Vertex;


	vec4 worldVertexPosition = Model * (animatedVertexPosition) ;
	fragPos = worldVertexPosition / worldVertexPosition.w;
	fragCol = Color;
	fragNormal = worldspaceNormal;
	texCoords = Texture;
	shininess = materialShininess;
	gl_Position = Projection * (View * worldVertexPosition);
}
