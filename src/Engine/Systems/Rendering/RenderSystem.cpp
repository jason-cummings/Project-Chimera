#include "RenderSystem.hpp"

RenderSystem::RenderSystem( int width, int height ) {
	glGenVertexArrays( 1, &BASE_VAO );
	glBindVertexArray( BASE_VAO );

	sm = ShaderManager::getShaderManager();
	reshape( width, height );
}

void RenderSystem::reshape( int new_width, int new_height ) {
	view_width = new_width;
	view_height = new_height;
	aspect_ratio = view_width / (float)view_height;
	createMatrices();
}

void RenderSystem::render( double dt ) {

	testGLError("Before");
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, view_width, view_height);

	testGLError("Clear");

	// Load the matrices to the shader
	Shader *ds = sm->getShader("default");
	ds->bind();
	testGLError("Shader");

	ds->setUniformMat4( "Model", model_mat );
	ds->setUniformMat4( "View", view_mat );
	ds->setUniformMat4( "Projection", proj_mat );
	testGLError("Mats");

	TEMP_cube.render();

	testGLError("Render");

	// End render
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);

	testGLError("EndRender");
}

void RenderSystem::createMatrices() {
	proj_mat = glm::perspective(glm::radians(fov), aspect_ratio , 0.1f, 100.f);
	view_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
	model_mat = glm::rotate(model_mat, glm::radians(TEMP_ph), glm::vec3(1.0f, 0.0f, 0.0f));
	model_mat = glm::rotate(model_mat, glm::radians(TEMP_th), glm::vec3(0.0f, 1.0f, 0.0f));
}

void RenderSystem::testGLError( const char *loc ) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cout << "OpenGL error at " << loc << ": " << err << std::endl;
}