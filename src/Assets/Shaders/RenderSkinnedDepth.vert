#version 460

layout(location = 0) in vec4 Vertex;
layout(location = 6) in vec4 Joint1to4;
layout(location = 7) in vec4 Joint5to8;
layout(location = 8) in vec4 Joint9to12;
layout(location = 9) in vec4 Joint13to16;
layout(location = 10) in vec4 Joint17to20;
layout(location = 11) in vec2 Joint21to22;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform mat4 boneMatrices[25];

void main() {
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

	gl_Position = Projection * View * Model * animatedVertexPosition;
}
