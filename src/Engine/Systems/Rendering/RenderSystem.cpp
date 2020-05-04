#include "RenderSystem.hpp"

#define SHADOW_MAP_DIMENSION 2048
#define BLOOM_PASSES 4

const float sun_distances[4] = { 5.f, 15.f, 40.f, 100.f };

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

	// texture_width = 2880;//3840;
	// texture_height = 1800;//2160;
	texture_width = 3840;
	texture_height = 2160;

	// Setup the necessary framebuffers for rendering
	createFramebuffers();

	// And in the last step, Jason said "Let there be light"
	// sun.location = glm::vec3(1.f,3.f,1.f);
	sun.location = glm::vec3(.707f,.3f,-.707f);
	sun.diffuse = glm::vec3(0.5f,0.3f,0.2f);
	sun.specular = glm::vec3(0.0f,0.0f,0.0f);
	sun.linear_attenuation = 0.08f;
	sun.quadratic_attenuation = 0.0f;
	sun.directional = 1.0f;
	sun_proj_mats[0] = glm::ortho( -sun_distances[0], sun_distances[0], -sun_distances[0], sun_distances[0], -100.f, 100.f );
	sun_proj_mats[1] = glm::ortho( -sun_distances[1], sun_distances[1], -sun_distances[1], sun_distances[1], -100.f, 100.f );
	sun_proj_mats[2] = glm::ortho( -sun_distances[2], sun_distances[2], -sun_distances[2], sun_distances[2], -100.f, 100.f );
	sun_proj_mats[3] = glm::ortho( -sun_distances[3], sun_distances[3], -sun_distances[3], sun_distances[3], -100.f, 100.f );

	shadow_mode = ShadowMode::SINGLE_PASS;
}

// Create and return the singleton instance of RenderSystem
RenderSystem & RenderSystem::getRenderSystem() {
	static RenderSystem rs;
	return rs;
}

void RenderSystem::reshape( int x_size, int y_size ) {
	view_width = x_size;
	view_height = y_size;
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
	deferred_buffer.addColorTextureHighPrecision( "position", texture_width, texture_height );
	deferred_buffer.addColorTexture( "normal", texture_width, texture_height );
	deferred_buffer.addColorTexture( "diffuse", texture_width, texture_height );
	deferred_buffer.addColorTexture( "emissive", texture_width, texture_height );
	deferred_buffer.addColorTexture( "occlusion", texture_width, texture_height );
	deferred_buffer.addDepthBuffer( texture_width, texture_height );

	// Add the depth texture for the shadow buffer
	depth_shadow_buffer.addDepthBuffer( 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	depth_shadow_buffer.addDepthTexture( "shadow_depth", 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );

	// Add an output shadow texture for the shadow mapping buffer
	shadow_mapping_buffer.addColorTexture( "shadow_map", texture_width, texture_height );
	shadow_mapping_buffer.addColorTexture( "secondary", texture_width, texture_height );
	shadow_mapping_buffer.addColorTexture( "three", texture_width, texture_height );
	shadow_mapping_buffer.addColorTexture( "four", texture_width, texture_height );

	// Set up the shading framebuffer
	shading_buffer.addColorTexture( "FragColor", texture_width, texture_height );
	shading_buffer.addColorTexture( "BrightColor", texture_width, texture_height );

	// Set up the pingpong buffers for blurring
	blur_buffer[0].addColorTexture( "FragColor", texture_width, texture_height );
	blur_buffer[1].addColorTexture( "FragColor", texture_width, texture_height );

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

void RenderSystem::drawDepthTexture( GLuint tex ) {
	// Bind the quad program
	Shader *draw_depth_shader = sm->getShader( "draw-depth-tex" );
	draw_depth_shader->bind();
	draw_depth_shader->setUniformInt( "depthMap", 0 );

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
		to_draw->draw();
	}
}

void RenderSystem::drawOverlayMeshList( bool useMaterials, Shader * shader ) {
	for( int i=0; i<overlay_mesh_list.size(); i++ ) {
		glm::mat4 transform = overlay_mesh_list[i]->getWorldTransform();
		shader->setUniformMat4( "Model", transform );

		OverlayMesh *to_draw = overlay_mesh_list[i]->getOverlayMesh();
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader, false );
		to_draw->draw();
	}
}

void RenderSystem::drawMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < mesh_list.size(); i++) {
		glm::mat4 transform = mesh_list[i]->getWorldTransform();
		shader->setUniformMat4( "Model", transform );
		mesh_list[i]->getMesh()->drawVerticesOnly();
	}
}

void RenderSystem::drawSkinnedMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < skinned_mesh_list.size(); i++) {
		glm::mat4 transform = skinned_mesh_list[i]->getWorldTransform();
		shader->setUniformMat4( "Model", transform );

		SkinnedMesh *to_draw = skinned_mesh_list[i]->getSkinnedMesh();

		JointList *joint_list = to_draw->getJointList();
		int num_bones = joint_list->getNumBones();
		std::vector<glm::mat4> *buffer = joint_list->getTransformMatrices();
		for(int i = 0; i < num_bones; i++) {
			shader->setUniformMat4("boneMatrices[" + std::to_string(i) + "]", (*buffer)[i]);
		}
		
		to_draw->drawVerticesOnly();
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
		createOrthoMatrices();
	}

	// Do the deferred rendering
	deferredRenderStep();

	if( shadow_mode != ShadowMode::NONE ) {
		// Render shadow maps
		renderDirectionalDepthTexture( &sun );
		createDirectionalShadowMap( &sun );
	}

	// Perform shading
	shadingStep();

	// Render overlays
	renderOverlay();

	// Draw the resulting texture from the shading step
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, view_width, view_height );
	glClear( GL_COLOR_BUFFER_BIT );
	drawTexture( shading_buffer.getTexture( "FragColor" )->getID() );

	applyBloom();

	glFinish();
	
	mesh_list.clear(); // this probably should be moved
	skinned_mesh_list.clear();
	overlay_mesh_list.clear();
}

void RenderSystem::populateRenderLists( GameObject * game_object ) {
	if(game_object->hasMesh()) {
		mesh_list.push_back(game_object);
	}
	if(game_object->hasSkinnedMesh()) {
		skinned_mesh_list.push_back(game_object);
	}
	if(game_object->hasOverlayMesh()) {
		overlay_mesh_list.push_back(game_object);
	}
	for(int i = 0; i < game_object->getNumChildren(); i++) {
		populateRenderLists(game_object->getChild(i));
	}
}

// Function to create default view and projection matrices only if the camera seg faults
void RenderSystem::createOrthoMatrices() {
	float aspect_ratio = view_width / (float)view_height;
	float left_edge = (aspect_ratio - 1.f) / -2.f;
	proj_mat = glm::ortho( left_edge, left_edge + aspect_ratio, 0.f, 1.f, -1.f, 1.f );
	view_mat = glm::mat4(1.f);
}


void RenderSystem::deferredRenderStep() {
	// bind framebuffer
	deferred_buffer.bind();

	// Clear the framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
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
	glDisable( GL_CULL_FACE );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	testGLError("Deferred Rendering");
}

// Render all meshes to the shadow buffer
void RenderSystem::renderDirectionalDepthTexture( Light *light ) {
	// Get the necessary shaders for this step
	Shader *depth_shader = sm->getShader("depth");
	Shader *skinned_depth_shader = sm->getShader("skinned-depth");

	// Create the view and projection matrices for the light's view
	glm::vec3 light_look_at = glm::vec3( camera->getWorldTransform()[3] );
	// glm::vec3 light_look_at = glm::vec3(0.f);
	glm::mat4 light_view_mat = glm::lookAt( light_look_at + light->location, light_look_at, glm::vec3(0.f,1.f,0.f) );

	// Bind and clear the depth only framebuffer
	depth_shadow_buffer.bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	if( shadow_mode == ShadowMode::ITERATE ) {
		for( int i=0; i<4; i++ ) {
			glViewport( SHADOW_MAP_DIMENSION*i, 0, SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );

			// Bind the shader and render everything for normal meshes
			depth_shader->bind();
			depth_shader->setUniformMat4( "View", light_view_mat );
			depth_shader->setUniformMat4( "Projection", sun_proj_mats[i] );
			drawMeshListVerticesOnly( depth_shader );
			
			// Bind the shader and render everything for skinned meshes
			skinned_depth_shader->bind();
			skinned_depth_shader->setUniformMat4( "View", light_view_mat );
			skinned_depth_shader->setUniformMat4( "Projection", sun_proj_mats[i] );
			drawSkinnedMeshListVerticesOnly( skinned_depth_shader );
		}
	}
	else {
		glViewport( SHADOW_MAP_DIMENSION*2, 0, SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
		// Bind the shader and render everything for normal meshes
		depth_shader->bind();
		depth_shader->setUniformMat4( "View", light_view_mat );
		depth_shader->setUniformMat4( "Projection", sun_proj_mats[2] );
		drawMeshListVerticesOnly( depth_shader );
		
		// Bind the shader and render everything for skinned meshes
		skinned_depth_shader->bind();
		skinned_depth_shader->setUniformMat4( "View", light_view_mat );
		skinned_depth_shader->setUniformMat4( "Projection", sun_proj_mats[2] );
		drawSkinnedMeshListVerticesOnly( skinned_depth_shader );
	}

	// End Render
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	testGLError("Depth Texture render");
}

// Create the shadow map texture to pass to the shading step
void RenderSystem::createDirectionalShadowMap( Light *light ) {
	// Get the necessary shaders for this step
	Shader *mapping_shader = sm->getShader("directional-shadows");

	// Create the view and projection matrices for the light's view
	glm::vec3 light_look_at = glm::vec3( camera->getWorldTransform()[3] );
	// glm::vec3 light_look_at = glm::vec3(0.f);
	glm::mat4 light_view_mat = glm::lookAt( light_look_at + light->location, light_look_at, glm::vec3(0.f,1.f,0.f) );

	// Bind and clear the mapping buffer
	shadow_mapping_buffer.bind();
	glClear( GL_COLOR_BUFFER_BIT );
	glViewport( 0, 0, texture_width, texture_height );

	mapping_shader->bind();

	// Bind the necessary textures for shadow mapping
	mapping_shader->setUniformInt( "positionTexture", 0 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "position" )->getID() );

	mapping_shader->setUniformInt( "normalTexture", 1 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, deferred_buffer.getTexture( "normal" )->getID() );
	
	mapping_shader->setUniformInt( "depthTexture", 2 );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, depth_shadow_buffer.getDepthTexture()->getID() );

	// Load the lightspace transform matrices
	mapping_shader->setUniformMat4( "lightView", light_view_mat );
	for( int i=0; i<4; i++ ) {
		mapping_shader->setUniformMat4( "lightProjections[" + std::to_string(i) + "]", sun_proj_mats[i] );
		mapping_shader->setUniformFloat( "lightDistanceThresholds[" + std::to_string(i) + "]", sun_distances[i] );
	}
	mapping_shader->setUniformVec3( "lightLocation", light->location );

	mapping_shader->setUniformVec3( "cameraLocation", light_look_at );

	mapping_shader->setUniformFloat( "iterate", shadow_mode == ShadowMode::ITERATE ? 1.0 : 0.0 );

	// Render the shadow map
	drawQuad();


	// // Blur the shadow map
	// Shader *blur_shader = sm->getShader("blur");
	// blur_shader->bind();
	// blur_shader->setUniformInt( "colorTexture", 0 );
	// glActiveTexture( GL_TEXTURE0 );

	// current_blur_buffer = 0;

	// for( int i=0; i<8; i++ ) { // BLOOM_PASSES
	// 	blur_shader->bind();
	// 	blur_buffer[current_blur_buffer].bind();
	// 	glViewport( 0, 0, texture_width, texture_height );
	// 	glClear( GL_COLOR_BUFFER_BIT );
		
	// 	blur_shader->setUniformFloat( "horizontal", (float)current_blur_buffer );
	// 	GLuint tex_to_use = i == 0 ? shadow_mapping_buffer.getTexture( "shadow_map" )->getID() : blur_buffer[!current_blur_buffer].getTexture( "FragColor" )->getID();
	// 	glBindTexture( GL_TEXTURE_2D, tex_to_use );

	// 	drawQuad();

	// 	current_blur_buffer = !current_blur_buffer;
	// }

	// shadow_mapping_buffer.bind();
	// drawTexture( blur_buffer[!current_blur_buffer].getTexture("FragColor")->getID() );

	// End Render
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	testGLError("Shadow Map render");
}

void RenderSystem::shadingStep() {
	// Bind the shading framebuffer
	shading_buffer.bind();

	glViewport( 0, 0, texture_width, texture_height );
	Shader *cartoon_shading = sm->getShader("cartoon");
	cartoon_shading->bind();

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
	
	cartoon_shading->setUniformInt("shadowTexture",4);
	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, shadow_mapping_buffer.getTexture( "shadow_map" )->getID());

	cartoon_shading->setUniformVec3("cameraLoc",glm::vec3(0.0f,0.0f,10.0f));

	cartoon_shading->setUniformFloat("ambientAmount", 0.3);

	cartoon_shading->setUniformFloat( "applyShadows", shadow_mode == ShadowMode::NONE ? 0.0 : 1.0 );

	// OLD LIGHT
	// cartoon_shading->setUniformVec3("light.location",glm::vec3(50.0f,100.0f,200.0f));
	// cartoon_shading->setUniformVec3("light.diffuse",glm::vec3(1.0f,1.0f,1.0f));
	// cartoon_shading->setUniformVec3("light.specular",glm::vec3(1.0f,1.0f,1.0f));
	// cartoon_shading->setUniformFloat("light.linearAttenuation",0.08f);
	// cartoon_shading->setUniformFloat("light.quadraticAttenuation",0.0f);
	// cartoon_shading->setUniformFloat("light.directional",0.0f);

	cartoon_shading->setUniformVec3( "light.location", sun.location );
	cartoon_shading->setUniformVec3( "light.diffuse", sun.diffuse );
	cartoon_shading->setUniformVec3( "light.specular", sun.specular );
	cartoon_shading->setUniformFloat( "light.linearAttenuation", sun.linear_attenuation );
	cartoon_shading->setUniformFloat( "light.quadraticAttenuation", sun.quadratic_attenuation );
	cartoon_shading->setUniformFloat( "light.directional", sun.directional );

	drawQuad();

	testGLError("Shading");
	
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
}


void RenderSystem::applyBloom() {
	Shader *blur_shader = sm->getShader("blur");
	blur_shader->bind();
	blur_shader->setUniformInt( "colorTexture", 0 );
	glActiveTexture( GL_TEXTURE0 );

	current_blur_buffer = 0;

	for( int i=0; i<BLOOM_PASSES; i++ ) { // BLOOM_PASSES
		blur_shader->bind();
		blur_buffer[current_blur_buffer].bind();
		glViewport( 0, 0, texture_width, texture_height );
		glClear( GL_COLOR_BUFFER_BIT );
		
		blur_shader->setUniformFloat( "horizontal", (float)current_blur_buffer );
		GLuint tex_to_use = i == 0 ? shading_buffer.getTexture( "BrightColor" )->getID() : blur_buffer[!current_blur_buffer].getTexture( "FragColor" )->getID();
		glBindTexture( GL_TEXTURE_2D, tex_to_use );

		drawQuad();

		current_blur_buffer = !current_blur_buffer;
	}

	testGLError("Blur");

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, view_width, view_height );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	drawTexture( blur_buffer[!current_blur_buffer].getTexture("FragColor")->getID() );
	glDisable( GL_BLEND );

	glUseProgram(0);
}

void RenderSystem::renderOverlay() {
	shading_buffer.bind();
	glViewport( 0, 0, texture_width, texture_height );
	Shader * overlay_shader = sm->getShader("overlay");
	overlay_shader->bind();

	// Create the orthographic matrices to render the overlay
	createOrthoMatrices();

	overlay_shader->setUniformMat4( "Projection", proj_mat );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	drawOverlayMeshList( true, overlay_shader );
	
	glDisable( GL_BLEND );

	testGLError("Overlay");

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
}
