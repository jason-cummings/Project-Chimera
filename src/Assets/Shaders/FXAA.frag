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

   // loop unrolled version of edge detection and gaussian blur.
   //    a 3x3 kernel is used for both, and they are done simultaneously to reduce the number 
   //    of calculations related to identifying the texels to sample


   // (-1,-1) to (-1,1) relative to this texel

   vec2 n_texCoords = texCoords + vec2(-1.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[0];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[0];

   n_texCoords = texCoords + vec2(-1.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[1];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[1];

   n_texCoords = texCoords + vec2(-1.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[2];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[2];


   // (0,-1) to (0,1) relative to this texel

   n_texCoords = texCoords + vec2(0.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[3];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[3];

   n_texCoords = texCoords + vec2(0.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[4];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[4];

   n_texCoords = texCoords + vec2(0.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[5];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[5];

   // (1,-1) to (1,1) relative to this texel

   n_texCoords = texCoords + vec2(1.0f,-1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[6];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[6];

   n_texCoords = texCoords + vec2(1.0f,0.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[7];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[7];

   n_texCoords = texCoords + vec2(1.0f,1.0f) * texelSize;
   edge_detection_result += texture(position_texture,n_texCoords).xyz * laplacian_weights[8];
   blur_result += texture(color_texture,n_texCoords).xyz * gaussian_weights[8];

   
   // detect edges and determine how much of the blur result should be used for the final image
   float blend_amount = min(length(edge_detection_result),1.0f);

   // mix the resulting color from the gaussian blur with the original color of the pixel. 
   // If the edge detection detects a strong edge, the blurred version. The weaker the edge, the
   // more the original color is used instead of the blurred version. If no edge was detected at
   // all, the original color is used 
   FragColor = vec4(mix(texture(color_texture, texCoords).xyz,blur_result,blend_amount),1.0f);
}
