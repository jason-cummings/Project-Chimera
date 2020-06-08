#include "Skybox.hpp"

#include "TextureLoader.hpp"

#include "../../Utilities/FilesystemWrapper.hpp"
#include "../../Utilities/WAIWrapper.hpp"
#include "../../Utilities/Asset.hpp"

bool SkyboxFactory::initialized = false;
GLuint SkyboxFactory::vao = 0;
GLuint SkyboxFactory::vbo = 0;

Skybox::Skybox(GLuint vao_in, GLuint vbo_in, GLuint cubemap_in) {
	vao = vao_in;
	vbo = vbo_in;
	cubemap = cubemap_in;
}

void Skybox::draw(Shader * shader) {

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(vao);

	shader->setUniformInt("skybox",0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

	// glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void SkyboxFactory::initializeVAO() {

	// glEnable(GL_TEXTURE_CUBE_MAP);
	//set up skybox vao
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// set up skybox vbo
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, &skybox_vbo_data, GL_STATIC_DRAW );

	glEnableVertexAttribArray( ShaderAttrib::Vertex );
	glVertexAttribPointer( ShaderAttrib::Vertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0));

	glBindVertexArray(0);
}

GLuint SkyboxFactory::createCubeMap(std::vector<fs::path> texture_names) {
	GLuint cubemap;
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    for(int i = 0; i < 6; i++) {
    	std::string temp = texture_names[i].string();
    	TextureLoader::loadTextureForCubeMap(temp, false, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cubemap;
}

Skybox * SkyboxFactory::getSkybox(std::string directory) {

	fs::path directory_path = Asset::assetPath().append(directory);
	if(!initialized) {
		initializeVAO();
	}

	std::vector<fs::path> texture_names;
	for(int i = 0; i < 6; i++) {
		fs::path tex_name = directory_path;
		tex_name.append(skybox_texture_names[i]);
		texture_names.push_back(tex_name);
	}

	GLuint cubemap = createCubeMap(texture_names);

	return new Skybox(vao,vbo,cubemap);
}