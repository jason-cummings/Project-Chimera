#include "RenderSystem.hpp"

RenderSystem::RenderSystem() {
	camera = nullptr;
	skybox = nullptr;

	// Create the basic VAO
	glGenVertexArrays( 1, &BASE_VAO );
	glBindVertexArray( BASE_VAO );

	//set up quad vao
	glGenVertexArrays( 1, &quad_vao );
	glBindVertexArray( quad_vao );
	
	// Create the VBO for the quad
	glGenBuffers( 1, &quad_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, quad_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 30, &quad_vbo_data, GL_STATIC_DRAW );

	glEnableVertexAttribArray( ShaderAttrib2D::Vertex2D );
	glEnableVertexAttribArray( ShaderAttrib2D::Texture2D );
	glVertexAttribPointer( ShaderAttrib2D::Vertex2D,  3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0) );
	glVertexAttribPointer( ShaderAttrib2D::Texture2D, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );

	glBindVertexArray(0);

	// Get the shader manager
	glBindVertexArray( BASE_VAO );
	sm = ShaderManager::getShaderManager();

	texture_width = 2880;//3840;
	texture_height = 1800;//2160;

	// Setup the necessary framebuffers for rendering
	createFramebuffers();
}

// Create and return the singleton instance of RenderSystem
RenderSystem & RenderSystem::getRenderSystem() {
	static RenderSystem rs;
	return rs;
}

/**
	Rendering Pipeline Setup
**/

// Wrapper function to catch GL errors
void RenderSystem::testGLError( const char *loc ) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cerr << "OpenGL error at " << loc << ": " << err << std::endl;
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
	glBindVertexArray( quad_vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
	testGLError("Quad");
}

void RenderSystem::drawMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < mesh_list.size(); i++) {
		glm::mat4 transform = mesh_list[i]->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );

		Mesh *to_draw = mesh_list[i]->getMesh();
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader );
		to_draw->draw();
	}
}

void RenderSystem::drawSkinnedMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < skinned_mesh_list.size(); i++) {
		glm::mat4 transform = skinned_mesh_list[i]->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );

		SkinnedMesh *to_draw = skinned_mesh_list[i]->getSkinnedMesh();

		JointList * joint_list = to_draw->getJointList();
		int num_bones = joint_list->getNumBones();
		std::vector<glm::mat4> * buffer = joint_list->getTransformMatrices();
		for(int i = 0; i < num_bones; i++) {
			shader->setUniformMat4("boneMatrices[" + std::to_string(i) + "]", (*buffer)[i]);
		}
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader );
		//TEMP_material->bind( shader );
		to_draw->draw();
	}
}


/**
	Rendering Pipeline
**/

void RenderSystem::render( double dt, GameObject * sceneGraph ) {
	//clear rendering lists
	populateRenderLists( sceneGraph );
	
	// Attempt to get the camera's matrices
	try {
		view_mat = camera->getViewMatrix();
		proj_mat = camera->getProjectionMatrix();
	} catch ( std::exception &e ) {
		// If failed, use some default matrices
		std::cerr << "Exception retrieving camera - using default matrices" << std::endl;
		createDefaultMatrices();
	}

	// Do the deferred rendering
	deferredRenderStep();

	// Perform shading
	shadingStep();

	//drawTexture(deferred_buffer.getTexture( "normal" )->getID());
}

void RenderSystem::populateRenderLists( GameObject * game_object ) {
	if(game_object->hasMesh()) {
		mesh_list.push_back(game_object);
	}
	if(game_object->hasSkinnedMesh()) {
		skinned_mesh_list.push_back(game_object);
	}
	for(int i = 0; i < game_object->getNumChildren(); i++) {
		populateRenderLists(game_object->getChild(i));
	}
}

// Function to create default view and projection matrices only if the camera seg faults
void RenderSystem::createDefaultMatrices() {
	float fov = 55.f;
	float aspect_ratio = 640.f/480.f;
	proj_mat = glm::perspective( glm::radians(fov), aspect_ratio , 1.f, 10000.f );
	view_mat = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 350.0f))); // 350.f
}


void RenderSystem::deferredRenderStep() {
	// bind framebuffer
	deferred_buffer.bind();

	// Clear the framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_CULL_FACE);
	glViewport( 0, 0, texture_width, texture_height );

	if(skybox) {
		Shader *skybox_shader = sm->getShader("skybox");
		skybox_shader->bind();
		glm::mat4 view_rot_and_scale = glm::mat4(glm::mat3(view_mat));
		skybox_shader->setUniformMat4( "View", view_rot_and_scale );
		skybox_shader->setUniformMat4( "Projection", proj_mat );

		skybox->draw(skybox_shader);
	}
	
	// Bind the shader for deferred rendering
	Shader *deferred_shader = sm->getShader("basic-deferred");
	deferred_shader->bind();
	

	// Load the matrices to the shader
	deferred_shader->setUniformMat4( "View", view_mat );
	deferred_shader->setUniformMat4( "Projection", proj_mat );

	// Set additional uniforms for the shader
	deferred_shader->setUniformFloat( "materialShininess", 32.f );

	

	// Perform rendering
	drawMeshList(true,deferred_shader);

	Shader *skinned_deferred_shader = sm->getShader("skinned-deferred");
	skinned_deferred_shader->bind();
	deferred_buffer.bind();

	skinned_deferred_shader->setUniformMat4( "View", view_mat );
	skinned_deferred_shader->setUniformMat4( "Projection", proj_mat );

	skinned_deferred_shader->setUniformFloat( "materialShininess", 32.f );

	drawSkinnedMeshList(true, skinned_deferred_shader);

	// Return to default framebuffer and program
	glDisable( GL_DEPTH_TEST );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	testGLError("Deferred Rendering");

	mesh_list.clear(); // this probably should be moved
	skinned_mesh_list.clear();
}

void RenderSystem::shadingStep() {

	int vw = 1920, vh = 1080;
	try {
		vw = camera->getViewWidth();
		vh = camera->getViewHeight();
	} catch( std::exception &e ) {
		std::cerr << "Error retrieving view width and height from camera" << std::endl;
	}

	glViewport( 0, 0, vw, vh );
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

	cartoon_shading->setUniformFloat("ambientAmount", .3);

	cartoon_shading->setUniformVec3("light.location",glm::vec3(50.0f,100.0f,200.0f));
	cartoon_shading->setUniformVec3("light.diffuse",glm::vec3(1.0f,1.0f,1.0f));
	cartoon_shading->setUniformVec3("light.specular",glm::vec3(1.0f,1.0f,1.0f));
	cartoon_shading->setUniformFloat("light.linearAttenuation",0.01f);
	cartoon_shading->setUniformFloat("light.quadraticAttenuation",0.0f);
	//cartoon_shading->setUniformFloat("light.directional",0.0f);

	testGLError("shading");

	drawQuad();
}
