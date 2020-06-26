#version 330 core

in vec2 texCoords;

layout (location = 0) out vec4 FragColor;

uniform sampler2D position_texture;
uniform sampler2D color_texture;

float weight[5] = float[5](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
vec2 texelSize = 1.0 / textureSize(position_texture, 0);

float laplacian_weights[9] = float[9](-1.0f,-1.0f,-1.0f,-1.0f,8.0f,-1.0f,-1.0f,-1.0f,-1.0f);
float gaussian_weights[9] = float[9](1.0f / 16.0f,2.0f / 16.0f,1.0f / 16.0f,
                                     2.0f / 16.0f,4.0f / 16.0f,2.0f / 16.0f,
                                     1.0f / 16.0f,2.0f / 16.0f,1.0f / 16.0f);

void main() {
   vec3 edge_detection_result = vec3(0.0f);
   vec3 blur_result = vec3(0.0f);

   // for(int i = -1; i < 2; i+=1) {
   //    for(int j = -1; j < 2; j+=1) {
   //       vec2 n_texCoords = texCoords + vec2(i,j) * texelSize;
   //       edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[3*(i+1) + j + 1];
   //       blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[3*(i+1) + j + 1];
   //    }
   // }

   vec2 n_texCoords = texCoords + vec2(-1.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[0];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[0];

   n_texCoords = texCoords + vec2(-1.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[1];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[1];

   n_texCoords = texCoords + vec2(-1.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[2];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[2];

   //

   n_texCoords = texCoords + vec2(0.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[3];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[3];

   n_texCoords = texCoords + vec2(0.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[4];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[4];

   n_texCoords = texCoords + vec2(0.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[5];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[5];

   //

   n_texCoords = texCoords + vec2(1.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[6];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[6];

   n_texCoords = texCoords + vec2(1.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[7];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[7];

   n_texCoords = texCoords + vec2(1.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[8];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[8];

   

   float blend_amount = min(length(edge_detection_result),1.0f);

   FragColor = vec4(mix(texture(color_texture, texCoords).xyz,blur_result,blend_amount),1.0f);
}
