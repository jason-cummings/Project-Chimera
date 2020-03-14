#include "RenderSystem.hpp"

RenderSystem::RenderSystem( int width, int height ) {
	// Create the basic VAO
	glGenVertexArrays( 1, &BASE_VAO );
	glBindVertexArray( BASE_VAO );
	
	// Create the VBO for the quad
	glGenBuffers( 1 , &quad_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, quad_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 30, &quad_vbo_data, GL_STATIC_DRAW );

	// Get the shader manager
	sm = ShaderManager::getShaderManager();
	reshape( width, height );

	// Setup the necessary framebuffers for rendering
	createFramebuffers();
}

void RenderSystem::reshape( int new_width, int new_height ) {
	view_width = new_width;
	view_height = new_height;
	aspect_ratio = view_width / (float)view_height;

	texture_width = 2048;
	texture_height = 1024;

	createMatrices();
}

/**
	Rendering Pipeline Setup
**/


void RenderSystem::testGLError( const char *loc ) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cout << "OpenGL error at " << loc << ": " << err << std::endl;
}


void RenderSystem::createFramebuffers() {
	// Add the color textures to render to in the deffered rendering step
	deferred_buffer.addColorTexture( "position", texture_width, texture_height );
	deferred_buffer.addColorTexture( "normal", texture_width, texture_height );
	deferred_buffer.addColorTexture( "diffuse", texture_width, texture_height );
	deferred_buffer.addColorTexture( "emissive", texture_width, texture_height );
	deferred_buffer.addColorTexture( "occlusion", texture_width, texture_height );
	deferred_buffer.addDepthBuffer( texture_width, texture_height );
	testGLError( "Framebuffer Setup" );
}


/**
	Rendering Pipeline Util Functions
**/


void RenderSystem::drawTexture( GLuint tex ) {
	// Bind the quad program
	Shader *quad_shader = sm->getShader( "quad" );
	quad_shader->bind();
	quad_shader->setUniformInt( "colorTexture", 0 );

	// Bind the texture to render
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );

	drawQuad();
}

void RenderSystem::drawQuad() {
	// Enable the 2D vertex attributes and bind the vbo data
    glEnableVertexAttribArray( ShaderAttrib2D::Vertex2D );
    glEnableVertexAttribArray( ShaderAttrib2D::Texture2D );
    glBindBuffer( GL_ARRAY_BUFFER, quad_vbo );
    glVertexAttribPointer( ShaderAttrib2D::Vertex2D,  3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttrib2D::Texture2D, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );

	// Draw
    glDrawArrays( GL_TRIANGLES, 0, 6 );

	// Disable everything
    glDisableVertexAttribArray( ShaderAttrib2D::Vertex2D );
    glDisableVertexAttribArray( ShaderAttrib2D::Texture2D );
	glUseProgram(0);
	testGLError("Quad");
}

void RenderSystem::drawMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < meshList.size(); i++) {
		glm::mat4 transform = meshList[i]->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );
		// if(i == 0)
		// 	meshList[i]->getMesh()->setUpDraw();
		meshList[i]->getMesh()->slowDraw();
		// if(i == meshList.size() - 1)
		// 	meshList[i]->getMesh()->cleanUpDraw();
	}
}


/**
	Rendering Pipeling
**/

void RenderSystem::render( double dt, GameObject * sceneGraph ) {
	//clear rendering lists
	populateRenderLists( sceneGraph );
	createMatrices();

	// Do the deferred rendering
	deferredRenderStep();

	shadingStep();

	// Display the diffuse texture for now
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// GLuint color_tex = deferred_buffer.getTexture( "normal" )->getID();
	// createMatrices();
	// glViewport( 0, 0, view_width, view_height );
	// drawTexture(color_tex);
}

void RenderSystem::populateRenderLists( GameObject * gameObject ) {
	if(gameObject->hasMesh()) {
		meshList.push_back(gameObject);
	}
	for(int i = 0; i < gameObject->getNumChildren(); i++) {
		populateRenderLists(gameObject->getChild(i));
	}
}

void RenderSystem::createMatrices() {
	proj_mat = glm::perspective(glm::radians(fov), aspect_ratio , 0.1f, 100.f);
	view_mat = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f)));
}


void RenderSystem::deferredRenderStep() {
	// Bind the shader and framebuffer for deferred rendering
	Shader *deferred_shader = sm->getShader("basic-deferred");
	deferred_shader->bind();
	deferred_buffer.bind();

	// Load the matrices to the shader
	deferred_shader->setUniformMat4( "View", view_mat );
	deferred_shader->setUniformMat4( "Projection", proj_mat );

	// Set additional uniforms for the shader
	deferred_shader->setUniformFloat( "materialShininess", 32.f );

	// Clear the framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_CULL_FACE);
	glViewport( 0, 0, texture_width, texture_height );

	// Perform rendering
	drawMeshList(true,deferred_shader);

	// Return to default framebuffer and program
	glDisable( GL_DEPTH_TEST );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	testGLError("Deferred Rendering");

	meshList.clear();
}

void RenderSystem::shadingStep() {

	glViewport( 0, 0, view_width, view_height );
	Shader *cartoon_shading = sm->getShader("cartoon");
	cartoon_shading->bind();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	cartoon_shading->setUniformInt("positionTexture",0);
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "position" )->getID());

	cartoon_shading->setUniformInt("normalTexture",1);
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "normal" )->getID());

	cartoon_shading->setUniformInt("diffuseTexture",2);
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "diffuse" )->getID());

	cartoon_shading->setUniformInt("emissiveTexture",3);
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "emissive" )->getID());

	cartoon_shading->setUniformVec3("cameraLoc",glm::vec3(0.0f,0.0f,10.0f));

	cartoon_shading->setUniformFloat("ambientAmount", .2);

	cartoon_shading->setUniformVec3("light.location",glm::vec3(10.0f,0.0f,10.0f));
	cartoon_shading->setUniformVec3("light.diffuse",glm::vec3(1.0f,1.0f,1.0f));
	cartoon_shading->setUniformVec3("light.specular",glm::vec3(1.0f,1.0f,1.0f));
	cartoon_shading->setUniformFloat("light.linearAttenuation",0.1f);
	cartoon_shading->setUniformFloat("light.quadraticAttenuation",0.0f);
	//cartoon_shading->setUniformFloat("light.directional",0.0f);

	testGLError("shading");

	drawQuad();
}
