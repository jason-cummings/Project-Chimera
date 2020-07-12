#include "RenderSystem.hpp"

#include <glm/mat3x3.hpp>

#include "../../Utilities/Asset.hpp"
#include "TextureLoader.hpp"
#include "../../SettingsManager.hpp"

#include "Mesh.hpp"
#include "SkinnedMesh.hpp"
#include "OverlayMesh.hpp"
#include "Material.hpp"

#define SHADOW_MAP_DIMENSION 2048
#define VARIANCE_SHADOW_MAP_DIMENSION 1024

const float sun_distances[4] = { 5.f, 15.f, 40.f, 100.f };
const float variance_sun_distances[4] = { 5.f, 10.f, 15.f, 100.f };

RenderSystem::RenderSystem() {
	camera = nullptr;
	skybox = nullptr;

	RenderUtils::init();

	// Get the shader manager
	// Create the basic VAO
	glGenVertexArrays( 1, &BASE_VAO );
	glBindVertexArray( BASE_VAO );
	sm = ShaderManager::getShaderManager();
	glBindVertexArray( 0 );

	// Assign values based on user settings
	texture_width = UserSettings::resolution_width;
	texture_height = UserSettings::resolution_height;

	RenderUtils::setTextureHeight(texture_height);
	RenderUtils::setTextureWidth(texture_width);

	// create post process objects
	FXAA_process = new FXAA( nullptr );

	vls_post_process = new VolumetricLightScattering( nullptr );

	bloom_post_process = new Bloom( nullptr );
	//bloom_post_process->createFrameBuffers();


	// Setup the necessary framebuffers for rendering
	addFramebufferTextures();

	// And in the last step, Jason said "Let there be light"
	sun.location = glm::vec3(.707f,.3f,-.707f);
	sun.diffuse = glm::vec3(0.5f,0.3f,0.2f);
	sun.specular = glm::vec3(0.5f,0.3f,0.2f);
	sun.linear_attenuation = 0.08f;
	sun.quadratic_attenuation = 0.0f;
	sun.directional = 1.0f;
	sun_proj_mats[0] = glm::ortho( -sun_distances[0], sun_distances[0], -sun_distances[0], sun_distances[0], -100.f, 100.f );
	sun_proj_mats[1] = glm::ortho( -sun_distances[1], sun_distances[1], -sun_distances[1], sun_distances[1], -100.f, 100.f );
	sun_proj_mats[2] = glm::ortho( -sun_distances[2], sun_distances[2], -sun_distances[2], sun_distances[2], -100.f, 100.f );
	sun_proj_mats[3] = glm::ortho( -sun_distances[3], sun_distances[3], -sun_distances[3], sun_distances[3], -100.f, 100.f );
	
	// Add intermediate bloom processes
	variance_blur_process = new Blur( variance_depth_shadow_buffer.getTexture( "depth" )->getID(), &variance_blurred_depth_out );
	variance_blur_process->setOutSize( VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_blur_process->createFrameBuffers();

	// Add blur process to final shadow map
	shadow_map_blur_process = new Blur( shadow_mapping_buffer.getTexture( "shadow_map" )->getID(), &shadow_mapping_buffer );
	shadow_map_blur_process->createFrameBuffers();
}

// Create and return the singleton instance of RenderSystem
RenderSystem & RenderSystem::getRenderSystem() {
	static RenderSystem rs;
	return rs;
}

void RenderSystem::reshape( int x_size, int y_size ) {
	view_width = x_size;
	view_height = y_size;
	RenderUtils::setViewHeight(y_size);
	RenderUtils::setViewWidth(x_size);
}

// To call on a change in render resolution
void RenderSystem::recreateFramebuffers() {
	texture_width = UserSettings::resolution_width;
	texture_height = UserSettings::resolution_height;
	if( camera ) {
		camera->setResolution( texture_width, texture_height );
	}

	deferred_buffer.clearAll();
	shading_buffer.clearAll();
	shadow_mapping_buffer.clearAll();
	depth_shadow_buffer.clearAll();

	bloom_post_process->clearFrameBufferTextures();

	addFramebufferTextures();
}

/**
	Rendering Pipeline Setup
**/


void RenderSystem::addFramebufferTextures() {
	// Add the color textures to render to in the deffered rendering step
	deferred_buffer.addColorTextureHighPrecision( "position", texture_width, texture_height );
	deferred_buffer.addColorTexture( "normal", texture_width, texture_height );
	deferred_buffer.addColorTexture( "diffuse", texture_width, texture_height );
	deferred_buffer.addColorTexture( "emissive", texture_width, texture_height );
	deferred_buffer.addColorTexture( "occlusion", texture_width, texture_height );
	deferred_buffer.addDepthBuffer( texture_width, texture_height );

	// Add the depth texture for the shadow buffer
	depth_shadow_buffer.addDepthBuffer( 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	depth_shadow_buffer.addDepthTexture( "depth", 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );

	// Add an output shadow texture for the shadow mapping buffer
	shadow_mapping_buffer.addColorTexture( "shadow_map", texture_width, texture_height );

	// Add the depth texture for the shadow buffer
	variance_depth_shadow_buffer.addDepthBuffer( VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_depth_shadow_buffer.addDepthTexture( "depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_depth_shadow_buffer.addColorTexture( "depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	
	// Add textures to blur depth textures
	variance_blurred_depth_out.addColorTexture( "blurred_depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );

	// Set up the shading framebuffer
	shading_buffer.addColorTexture( "FragColor", texture_width, texture_height );
	shading_buffer.addColorTexture( "BrightColor", texture_width, texture_height );

	bloom_post_process->setBrightTexture(shading_buffer.getTexture("BrightColor")->getID());
	bloom_post_process->createFrameBuffers();

	vls_post_process->setOcclusionTexture( deferred_buffer.getTexture( "occlusion" )->getID() );

	((FXAA*)FXAA_process)->setPositionTexture( deferred_buffer.getTexture( "position" )->getID() );
	((FXAA*)FXAA_process)->setColorTexture( shading_buffer.getTexture( "FragColor" )->getID() );

	RenderUtils::testGLError( "Framebuffer Textures" );
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

	RenderUtils::drawQuad();
}

void RenderSystem::drawDepthTexture( GLuint tex ) {
	// Bind the quad program
	Shader *draw_depth_shader = sm->getShader( "draw-depth-tex" );
	draw_depth_shader->bind();
	draw_depth_shader->setUniformInt( "depthMap", 0 );

	// Bind the texture to render
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );

	RenderUtils::drawQuad();
}

void RenderSystem::drawMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < mesh_list.size(); i++) {
		glm::mat4 transform = mesh_list[i]->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );

		Mesh *to_draw = (Mesh *) mesh_list[i]->getRenderable();
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

		SkinnedMesh *to_draw = (SkinnedMesh *) skinned_mesh_list[i]->getRenderable();

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

		OverlayMesh *to_draw = (OverlayMesh *) overlay_mesh_list[i]->getRenderable();
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader, false );
		to_draw->draw();
	}
}

void RenderSystem::drawMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < mesh_list.size(); i++) {
		glm::mat4 transform = mesh_list[i]->getWorldTransform();
		shader->setUniformMat4( "Model", transform );
		Mesh *to_draw = (Mesh *) mesh_list[i]->getRenderable();
		to_draw->drawVerticesOnly();
	}
}

void RenderSystem::drawSkinnedMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < skinned_mesh_list.size(); i++) {
		glm::mat4 transform = skinned_mesh_list[i]->getWorldTransform();
		shader->setUniformMat4( "Model", transform );

		SkinnedMesh *to_draw = (SkinnedMesh *) skinned_mesh_list[i]->getRenderable();

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

void RenderSystem::render( double dt ) {
	
	// Attempt to get the camera's matrices
	if( camera != nullptr ) {
		view_mat = camera->getViewMatrix();
		proj_mat = camera->getProjectionMatrix();
		camera_loc = glm::vec3(camera->getEyePos());
	}
	else {
		// std::cerr << "Camera is null - using default matrices" << std::endl;
		createOrthoMatrices();
	}

	// Do the deferred rendering
	deferredRenderStep();

	if( UserSettings::shadow_mode != ShadowMode::NONE ) {
		// Render shadow maps
		if( UserSettings::shadow_mode == ShadowMode::VARIANCE ) {
			renderVarianceDirectionalDepthTexture( &sun );
			createDirectionalVarianceShadowMap( &sun );
		}
		else {
			renderDirectionalDepthTexture( &sun );
			createDirectionalShadowMap( &sun );
		}
	}

	// Perform shading
	shadingStep();

	// Draw the resulting texture from the shading step
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, view_width, view_height );
	glClear( GL_COLOR_BUFFER_BIT );

	if( UserSettings::use_FXAA )
		FXAA_process->apply();
	else drawTexture( shading_buffer.getTexture( "FragColor" )->getID() );
	
	if( UserSettings::use_volumetric_light_scattering ) {

		// calculate and set sun screen space location
		glm::mat4 view_rot_and_scale = glm::mat4(glm::mat3(view_mat));
		glm::vec4 light_screen_space_vec4 = proj_mat * view_rot_and_scale * glm::vec4((glm::normalize(sun.location) * 1.0f),1.0);

		light_screen_space_vec4 = light_screen_space_vec4 / light_screen_space_vec4[3];
		light_screen_space_vec4 += glm::vec4(1.0,1.0,0.0,0.0);
		light_screen_space_vec4 = light_screen_space_vec4 * .5f;
		
		glm::vec2 sun_screen_loc = glm::vec2(light_screen_space_vec4);

		vls_post_process->setSunScreenCoords(sun_screen_loc);
		vls_post_process->apply();
	}
	
	if( UserSettings::bloom_mode != BloomMode::NONE ) {
		bloom_post_process->apply();
	}
	
	// Render overlays
	renderOverlay();

	glFinish();
}

void RenderSystem::populateRenderLists( GameObject * game_object ) {
	addToRenderLists(game_object);

	for(int i = 0; i < game_object->getNumChildren(); i++) {
		populateRenderLists(game_object->getChild(i));
	}
}

// non recursive
void RenderSystem::addToRenderLists( GameObject * game_object ) {
	if(game_object->hasRenderable()) {
		Renderable * game_object_renderable = game_object->getRenderable();
		RenderableType type = game_object_renderable->getType();

		switch(type) {
			case RenderableType::MESH:
				mesh_list.push_back(game_object);
				break;
			case RenderableType::SKINNED_MESH:
				skinned_mesh_list.push_back(game_object);
				break;
			case RenderableType::OVERLAY:
			{
				// Insert overlay meshes acording to z level to ensure in order rendering
				int insert_z = ((OverlayMesh*)game_object_renderable)->getZLevel(); 

				std::vector<GameObject*>::iterator it = overlay_mesh_list.begin();
				bool inserted = false;
				while( !inserted && it < overlay_mesh_list.end() ) {
					int current_z = ((OverlayMesh*)((*it)->getRenderable()))->getZLevel();
					if( insert_z > current_z ) {
						it = overlay_mesh_list.insert( it, game_object );
						inserted = true;
					}
					it++;
				}
				if( !inserted ) {
					overlay_mesh_list.push_back( game_object );
				}
				break;
			}
			default:
				break;
		}
	}
}

void RenderSystem::clearRenderLists() {
	mesh_list.clear();
	skinned_mesh_list.clear();
	overlay_mesh_list.clear();
}

void RenderSystem::removeGameObjectFromRenderLists(GameObject * game_object) {
	std::vector<GameObject*>::iterator it;

	it = std::remove( mesh_list.begin(), mesh_list.end(), game_object );
	if( it != mesh_list.end() ) mesh_list.erase( it );
	
	it = std::remove( skinned_mesh_list.begin(), skinned_mesh_list.end(), game_object );
	if( it != skinned_mesh_list.end() ) skinned_mesh_list.erase( it );
	
	it = std::remove( overlay_mesh_list.begin(), overlay_mesh_list.end(), game_object );
	if( it != overlay_mesh_list.end() ) overlay_mesh_list.erase( it );
}

void RenderSystem::removeGameObjectFromRenderListsRecursive(GameObject * game_object) {
	removeGameObjectFromRenderLists(game_object);

	for(int i = 0; i < game_object->getNumChildren(); i++) {
		removeGameObjectFromRenderListsRecursive(game_object->getChild(i));
	}
}

// Function to create default view and projection matrices only if the camera seg faults
void RenderSystem::createOrthoMatrices() {
	float aspect_ratio = RenderUtils::getViewWidth() / (float)RenderUtils::getViewHeight();
	float left_edge = (aspect_ratio - 1.f) / -2.f;
	proj_mat = glm::ortho( left_edge, left_edge + aspect_ratio, 0.f, 1.f, -1.f, 1.f );
	view_mat = glm::mat4(1.f);
	camera_loc = glm::vec3(0.f);
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
		skybox_shader->setUniformFloat( "threshold", .93f );
		// Proper usage of the shader:
		// skybox_shader->setUniformVec3( "toLight", glm::normalize(sun.location) );

		// currently using hardcoded value to match low angle of skybox's sun, which does not match
		// the actual light of the scene
		skybox_shader->setUniformVec3( "toLight", glm::normalize(glm::vec3(1.0f,.2f,-1.0f)) );


		skybox->draw(skybox_shader);
	}
	
	// Bind the shader for deferred rendering
	Shader *deferred_shader = sm->getShader("basic-deferred");
	deferred_shader->bind();
	

	// Load the matrices to the shader
	deferred_shader->setUniformMat4( "View", view_mat );
	deferred_shader->setUniformMat4( "Projection", proj_mat );

	// Set additional uniforms for the shader
	deferred_shader->setUniformFloat( "materialShininess", 2.f );


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
	RenderUtils::testGLError("Deferred Rendering");
}

// Render all meshes to the shadow buffer
void RenderSystem::renderDirectionalDepthTexture( Light *light ) {
	// Get the necessary shaders for this step
	Shader *depth_shader = sm->getShader("depth");
	Shader *skinned_depth_shader = sm->getShader("skinned-depth");

	// Create the view matricx for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->location, camera_loc, glm::vec3(0.f,1.f,0.f) );

	// Bind and clear the depth only framebuffer
	depth_shadow_buffer.bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	if( UserSettings::shadow_mode == ShadowMode::ITERATE ) {
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
	RenderUtils::testGLError("Depth Texture render");
}

// Create the shadow map texture to pass to the shading step
void RenderSystem::createDirectionalShadowMap( Light *light ) {
	// Get the necessary shaders for this step
	Shader *mapping_shader = sm->getShader("directional-shadows");

	// Create the view and projection matrices for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->location, camera_loc, glm::vec3(0.f,1.f,0.f) );

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

	mapping_shader->setUniformVec3( "cameraLocation", camera_loc );

	mapping_shader->setUniformFloat( "iterate", UserSettings::shadow_mode == ShadowMode::ITERATE ? 1.f : 0.f );

	// Render the shadow map
	RenderUtils::drawQuad();

	// Blur the resulting shadow map
	if( UserSettings::blur_shadow_map ) {
		shadow_map_blur_process->apply();
	}

	// End Render
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	RenderUtils::testGLError("Shadow Map render");
}

// Render all meshes to the shadow buffer
void RenderSystem::renderVarianceDirectionalDepthTexture( Light *light ) {
	// Get the necessary shaders for this step
	Shader *depth_shader = sm->getShader("depth");
	Shader *skinned_depth_shader = sm->getShader("skinned-depth");

	// Create the view matricx for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->location, camera_loc, glm::vec3(0.f,1.f,0.f) );
	glm::mat4 variance_sun_proj_mat = glm::ortho( -variance_sun_distances[2], variance_sun_distances[2], -variance_sun_distances[2], variance_sun_distances[2], -100.f, 100.f );

	// Bind and clear the depth only framebuffer
	variance_depth_shadow_buffer.bind();
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glViewport( 0, 0, VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );

	// Bind the shader and render everything for normal meshes
	depth_shader->bind();
	depth_shader->setUniformMat4( "View", light_view_mat );
	depth_shader->setUniformMat4( "Projection", variance_sun_proj_mat );
	drawMeshListVerticesOnly( depth_shader );
	
	// Bind the shader and render everything for skinned meshes
	skinned_depth_shader->bind();
	skinned_depth_shader->setUniformMat4( "View", light_view_mat );
	skinned_depth_shader->setUniformMat4( "Projection", variance_sun_proj_mat );
	drawSkinnedMeshListVerticesOnly( skinned_depth_shader );

	// End Render
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	RenderUtils::testGLError("Variance Depth Texture render");
}

// Create the shadow map texture to pass to the shading step
void RenderSystem::createDirectionalVarianceShadowMap( Light *light ) {
	// Blurs the depth texture from the variance_depth_shadow_buffer Framebuffer
	// The resulting blurred texture is "blurred_depth" in the variance_blurred_depth_out Framebuffer
	variance_blur_process->apply();

	// Get the necessary shaders for this step
	Shader *mapping_shader = sm->getShader("variance-shadows");

	// Create the view and projection matrices for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->location, camera_loc, glm::vec3(0.f,1.f,0.f) );
	glm::mat4 variance_sun_proj_mat = glm::ortho( -variance_sun_distances[2], variance_sun_distances[2], -variance_sun_distances[2], variance_sun_distances[2], -100.f, 100.f );

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
	glBindTexture( GL_TEXTURE_2D, variance_blurred_depth_out.getTexture( "blurred_depth" )->getID() );

	// Load the lightspace transform matrices
	mapping_shader->setUniformMat4( "lightView", light_view_mat );
	mapping_shader->setUniformMat4( "lightProjection", variance_sun_proj_mat );
	mapping_shader->setUniformVec3( "lightLocation", light->location );

	// Render the shadow map
	RenderUtils::drawQuad();

	// Blur the resulting shadow map
	if( UserSettings::blur_shadow_map ) {
		shadow_map_blur_process->apply();
	}

	// End Render
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
	RenderUtils::testGLError("Variance Shadow Map render");
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

	cartoon_shading->setUniformVec3( "cameraLoc", camera_loc );

	cartoon_shading->setUniformFloat("ambientAmount", 0.3f );

	cartoon_shading->setUniformFloat( "applyShadows", UserSettings::shadow_mode == ShadowMode::NONE ? 0.f : 1.f );

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

	RenderUtils::drawQuad();

	RenderUtils::testGLError("Shading");
	
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
}

void RenderSystem::renderOverlay() {
	// shading_buffer.bind();
	// glViewport( 0, 0, texture_width, texture_height );
	glViewport( 0, 0, RenderUtils::getViewWidth(), RenderUtils::getViewHeight() );
	Shader * overlay_shader = sm->getShader("overlay");
	overlay_shader->bind();

	// Create the orthographic matrices to render the overlay
	
	float aspect_ratio = view_width / (float)view_height;
	float left_edge = (aspect_ratio - 1.f) / -2.f;
	glm::mat4 overlay_proj_mat = glm::ortho( left_edge, left_edge + aspect_ratio, 0.f, 1.f, -1.f, 1.f );

	overlay_shader->setUniformMat4( "Projection", overlay_proj_mat );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	drawOverlayMeshList( true, overlay_shader );
	
	glDisable( GL_BLEND );

	RenderUtils::testGLError("Overlay");

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glUseProgram(0);
}
