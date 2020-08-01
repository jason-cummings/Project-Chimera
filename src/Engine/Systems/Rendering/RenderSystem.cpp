#include "RenderSystem.hpp"

#include <glm/mat3x3.hpp>

#include "../../Utilities/Asset.hpp"
#include "TextureLoader.hpp"
#include "../../SettingsManager.hpp"

#include "Mesh.hpp"
#include "SkinnedMesh.hpp"
#include "OverlayMesh.hpp"
#include "Material.hpp"

#include "MeshList/NoSortMeshList.hpp"
#include "MeshList/FrontToBackMeshList.hpp"
#include "MeshList/BackToFrontMeshList.hpp"

#define SHADOW_MAP_DIMENSION 2048
#define VARIANCE_SHADOW_MAP_DIMENSION 1024

const float variance_sun_distances[4] = { 5.f, 10.f, 15.f, 100.f };

RenderSystem::RenderSystem() {
	camera = nullptr;
	skybox = nullptr;

	RenderUtils::init();
	RenderUtils::setTextureHeight( UserSettings::resolution_height );
	RenderUtils::setTextureWidth( UserSettings::resolution_width );

	// Get the shader manager
	// Create the basic VAO
	glGenVertexArrays( 1, &BASE_VAO );
	glBindVertexArray( BASE_VAO );
	sm = ShaderManager::getShaderManager();
	glBindVertexArray( 0 );

	// create post process objects
	FXAA_process = new FXAA( &composite_buffer );
	vls_post_process = new VolumetricLightScattering( &composite_buffer );
	bloom_post_process = new Bloom( &composite_buffer );

	// Setup the necessary framebuffers for rendering
	addFramebufferTextures();

	// Add intermediate bloom processes
	variance_blur_process = new Blur( variance_depth_shadow_buffer.getTexture( "depth" )->getID(), &variance_blurred_depth_out );
	variance_blur_process->setOutSize( VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_blur_process->createFrameBuffers();

	mesh_list = (MeshList *) new FrontToBackMeshList();
	skinned_mesh_list = (MeshList *) new FrontToBackMeshList();
	overlay_mesh_list = (MeshList *) new NoSortMeshList();

	setupShaders();
}

// Create and return the singleton instance of RenderSystem
RenderSystem & RenderSystem::getRenderSystem() {
	static RenderSystem rs;
	return rs;
}

void RenderSystem::reshape( int x_size, int y_size ) {
	RenderUtils::setViewHeight(y_size);
	RenderUtils::setViewWidth(x_size);

	// Create the orthographic matrices to render the overlay
	float aspect_ratio = RenderUtils::getTextureWidth() / (float)RenderUtils::getTextureHeight();
	float left_edge = (aspect_ratio - 1.f) / -2.f;
	glm::mat4 overlay_proj_mat = glm::ortho( left_edge, left_edge + aspect_ratio, 0.f, 1.f, -1.f, 1.f );
	sm->getShader("overlay")->bind();
	sm->getShader("overlay")->setUniformMat4( "Projection", overlay_proj_mat );
	glUseProgram(0);
}

// To call on a change in render resolution
void RenderSystem::recreateFramebuffers() {
	if( camera ) {
		camera->setResolution( UserSettings::resolution_width, UserSettings::resolution_height );
	}

	deferred_buffer.clearAll();
	shading_buffer.clearAll();
	shadow_mapping_buffer.clearAll();
	depth_shadow_buffer.clearAll();

	bloom_post_process->clearFrameBufferTextures();
	vls_post_process->clearFrameBufferTextures();
	FXAA_process->clearFrameBufferTextures();

	addFramebufferTextures();
}

/**
	Rendering Pipeline Setup
**/


void RenderSystem::addFramebufferTextures() {
	// Add the color textures to render to in the deffered rendering step
	deferred_buffer.addColorTextureHighPrecision( "position", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	deferred_buffer.addColorTexture( "normal", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	deferred_buffer.addColorTextureHighPrecision( "diffuse", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	deferred_buffer.addColorTextureHighPrecision( "emissive", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	deferred_buffer.addColorTexture( "occlusion", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	deferred_buffer.addDepthBuffer( RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	// Add the depth texture for the shadow buffer
	depth_shadow_buffer.addDepthBuffer( 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	depth_shadow_buffer.addDepthTexture( "depth", 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );

	// Add an output shadow texture for the shadow mapping buffer
	shadow_mapping_buffer.addColorTexture( "shadow_map", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	// Add the depth texture for the shadow buffer
	variance_depth_shadow_buffer.addDepthBuffer( VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_depth_shadow_buffer.addDepthTexture( "depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	variance_depth_shadow_buffer.addColorTexture( "depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );
	
	// Add textures to blur depth textures
	variance_blurred_depth_out.addColorTexture( "blurred_depth", VARIANCE_SHADOW_MAP_DIMENSION, VARIANCE_SHADOW_MAP_DIMENSION );

	// Set up the shading framebuffer
	shading_buffer.addColorTextureHighPrecision( "FragColor", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	shading_buffer.addColorTextureHighPrecision( "BrightColor", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	
	composite_buffer.addColorTextureHighPrecision( "composite_texture", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	bloom_post_process->setBrightTexture(shading_buffer.getTexture("BrightColor")->getID());
	bloom_post_process->createFrameBuffers();

	vls_post_process->setOcclusionTexture( deferred_buffer.getTexture( "occlusion" )->getID() );

	FXAA_process->setPositionTexture( deferred_buffer.getTexture( "position" )->getID() );
	FXAA_process->setColorTexture( shading_buffer.getTexture( "FragColor" )->getID() );

	RenderUtils::testGLError( "Framebuffer Textures" );
}

// Set uniform values of shaders that will not change
void RenderSystem::setupShaders() {
	Shader *dir_shadows_shader = sm->getShader("directional-shadows");
	dir_shadows_shader->bind();
	dir_shadows_shader->setUniformInt( "positionTexture", 0 );
	dir_shadows_shader->setUniformInt( "normalTexture", 1 );
	dir_shadows_shader->setUniformInt( "depthTexture", 2 );

	Shader *variance_shadows_shader = sm->getShader("variance-shadows");
	variance_shadows_shader->bind();
	variance_shadows_shader->setUniformInt( "positionTexture", 0 );
	variance_shadows_shader->setUniformInt( "normalTexture", 1 );
	variance_shadows_shader->setUniformInt( "depthTexture", 2 );	

	Shader *cartoon_shader = sm->getShader("cartoon");
	cartoon_shader->bind();
	cartoon_shader->setUniformInt( "positionTexture", 0 );
	cartoon_shader->setUniformInt( "normalTexture", 1 );	
	cartoon_shader->setUniformInt( "diffuseTexture", 2 );	
	cartoon_shader->setUniformInt( "emissiveTexture", 3 );	
	cartoon_shader->setUniformInt( "shadowTexture", 4);
	
	Shader *emissive_shader = sm->getShader("emissive");
	emissive_shader->bind();
	emissive_shader->setUniformInt( "emissiveTexture", 0 );

	Shader *dir_light_shader = sm->getShader("directional-light");
	dir_light_shader->bind();
	dir_light_shader->setUniformInt( "positionTexture", 0 );
	dir_light_shader->setUniformInt( "normalTexture", 1 );	
	dir_light_shader->setUniformInt( "diffuseTexture", 2 );	
	dir_light_shader->setUniformInt( "shadowTexture", 3 );
	dir_light_shader->setUniformFloat( "shadeCartoon", 0.f );

	Shader *point_light_shader = sm->getShader("point-light");
	point_light_shader->bind();
	point_light_shader->setUniformInt( "positionTexture", 0 );
	point_light_shader->setUniformInt( "normalTexture", 1 );	
	point_light_shader->setUniformInt( "diffuseTexture", 2 );	
	// point_light_shader->setUniformInt( "shadowTexture", 3 );
	point_light_shader->setUniformFloat( "shadeCartoon", 0.f );

	Shader *quad_shader = sm->getShader( "quad" );
	quad_shader->bind();
	quad_shader->setUniformInt( "colorTexture", 0 );

	Shader *draw_depth_shader = sm->getShader( "draw-depth-tex" );
	draw_depth_shader->bind();
	draw_depth_shader->setUniformInt( "depthMap", 0 );

	Shader *correction_shader = sm->getShader( "hdr-gamma" );
	correction_shader->bind();
	correction_shader->setUniformInt( "colorTexture", 0 );

	RenderUtils::testGLError("Shader setup");

	glUseProgram(0);
}


RenderSystem::~RenderSystem() {
	delete mesh_list;
	delete skinned_mesh_list;
	delete overlay_mesh_list;
}


/**
	Rendering Pipeline Util Functions
**/


void RenderSystem::drawTexture( GLuint tex ) {
	// Bind the quad program
	Shader *quad_shader = sm->getShader( "quad" );
	quad_shader->bind();

	// Bind the texture to render
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );

	RenderUtils::drawQuad();
}

void RenderSystem::drawDepthTexture( GLuint tex ) {
	// Bind the quad program
	Shader *draw_depth_shader = sm->getShader( "draw-depth-tex" );
	draw_depth_shader->bind();

	// Bind the texture to render
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );

	RenderUtils::drawQuad();
}

void RenderSystem::drawMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < mesh_list->size(); i++) {
		GameObject * obj = sorted? mesh_list->quickGet(i) : mesh_list->longGet(i);
		glm::mat4 transform = obj->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );

		Mesh *to_draw = (Mesh *) obj->getRenderable();
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader );
		to_draw->draw();
	}
}

void RenderSystem::drawSkinnedMeshList(bool useMaterials, Shader * shader) {
	for(int i = 0; i < skinned_mesh_list->size(); i++) {

		GameObject * obj = sorted? skinned_mesh_list->quickGet(i) : skinned_mesh_list->longGet(i);

		glm::mat4 transform = obj->getWorldTransform();
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(transform)));
		shader->setUniformMat4( "Model", transform );
		shader->setUniformMat3( "NormalMatrix", normal_matrix );

		SkinnedMesh *to_draw = (SkinnedMesh *) obj->getRenderable();

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
	for( int i=0; i<overlay_mesh_list->size(); i++ ) {

		GameObject * obj = sorted? overlay_mesh_list->quickGet(i) : overlay_mesh_list->longGet(i);

		glm::mat4 transform = obj->getWorldTransform();
		shader->setUniformMat4( "Model", transform );

		OverlayMesh *to_draw = (OverlayMesh *) obj->getRenderable();
		Material *mat_to_use = to_draw->getMaterial();
		
		mat_to_use->bind( shader, false );
		to_draw->draw();
	}
}

void RenderSystem::drawMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < mesh_list->size(); i++) {

		GameObject * obj = mesh_list->quickGet(i);

		glm::mat4 transform = obj->getWorldTransform();
		shader->setUniformMat4( "Model", transform );
		Mesh *to_draw = (Mesh *) obj->getRenderable();
		to_draw->drawVerticesOnly();
	}
}

void RenderSystem::drawSkinnedMeshListVerticesOnly( Shader * shader ) {
	for(int i = 0; i < skinned_mesh_list->size(); i++) {

		GameObject * obj = skinned_mesh_list->quickGet(i);

		glm::mat4 transform = obj->getWorldTransform();
		shader->setUniformMat4( "Model", transform );

		SkinnedMesh *to_draw = (SkinnedMesh *) obj->getRenderable();

		JointList *joint_list = to_draw->getJointList();
		int num_bones = joint_list->getNumBones();
		std::vector<glm::mat4> *buffer = joint_list->getTransformMatrices();
		for(int i = 0; i < num_bones; i++) {
			shader->setUniformMat4("boneMatrices[" + std::to_string(i) + "]", (*buffer)[i]);
		}
		
		to_draw->drawVerticesOnly();
	}
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
				mesh_list->addGameObject(game_object);
				break;
			case RenderableType::SKINNED_MESH:
				skinned_mesh_list->addGameObject(game_object);
				break;
			case RenderableType::OVERLAY:
			{
				// Insert overlay meshes acording to z level to ensure in order rendering
				int insert_z = ((OverlayMesh*)game_object_renderable)->getZLevel(); 

				int i = 0;
				bool inserted = false;
				while(!inserted && i < overlay_mesh_list->size()) {
					int current_z = ((OverlayMesh *)(overlay_mesh_list->quickGet(i)->getRenderable()))->getZLevel();
					if( insert_z > current_z ) {
						((NoSortMeshList*)overlay_mesh_list)->addGameObject(game_object,i);
						inserted = true;
					}
					i++;
				}
				if( !inserted ) {
					overlay_mesh_list->addGameObject( game_object );
				}
				break;
			}
			default:
				break;
		}
	}
}

void RenderSystem::clearRenderLists() {
	mesh_list->clearList();
	skinned_mesh_list->clearList();
	overlay_mesh_list->clearList();
}

void RenderSystem::removeGameObjectFromRenderLists(GameObject * game_object) {
	mesh_list->removeObject(game_object);
	skinned_mesh_list->removeObject(game_object);
	overlay_mesh_list->removeObject(game_object);
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





/**************************************
 *    RENDERING PIPELINE FUNCTIONS    *
 **************************************/

void RenderSystem::render( double dt ) {
	
	// Attempt to get the camera's matrices
	if( camera != nullptr ) {
		view_mat = camera->getViewMatrix();
		proj_mat = camera->getProjectionMatrix();
		camera_loc = glm::vec3(camera->getEyePos());
		shadow_focal_point = glm::vec3( camera->getWorldTransform()[3] );

		mesh_list->setCameraLoc(camera_loc);
		skinned_mesh_list->setCameraLoc(camera_loc);
	}
	else {
		// std::cerr << "Camera is null - using default matrices" << std::endl;
		createOrthoMatrices();
	}

	sorted = false; // will change with material overhaul

	// Do the deferred rendering
	deferredRenderStep();

	sorted = true;

	// if( UserSettings::shadow_mode != ShadowMode::NONE ) {
	// 	// Render shadow maps
	// 	if( UserSettings::shadow_mode == ShadowMode::VARIANCE ) {
	// 		renderDirectionalVarianceDepthTexture( directional_lights[0] );
	// 		createDirectionalVarianceShadowMap( directional_lights[0] );
	// 	}
	// 	else {
	// 		renderDirectionalDepthTexture( directional_lights[0] );
	// 		createDirectionalShadowMap( directional_lights[0] );
	// 	}
	// }

	createShadowMaps();

	// Perform shading
	if( UserSettings::new_shading ) {
		performShading();
	}
	else {
		shadingStep();
	}

	// Draw the resulting texture from the shading step
	composite_buffer.bind();
	glViewport( 0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glViewport( 0, 0, RenderUtils::getViewWidth(), RenderUtils::getViewHeight() );

	glClear( GL_COLOR_BUFFER_BIT );

	if( UserSettings::use_FXAA )
		FXAA_process->apply();
	else drawTexture( shading_buffer.getTexture( "FragColor" )->getID() );

	if( UserSettings::use_volumetric_light_scattering && directional_lights.size() > 0 ) {

		// calculate and set sun screen space location
		glm::mat4 view_rot_and_scale = glm::mat4(glm::mat3(view_mat));
		glm::vec4 light_screen_space_vec4 = proj_mat * view_rot_and_scale * glm::vec4((glm::normalize( directional_lights[0]->getLocation() ) * 1.0f),1.0);

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

	// Render the final result
	correctAndRenderFinal();

	glFinish();
}

void RenderSystem::deferredRenderStep() {
	// bind framebuffer
	deferred_buffer.bind();

	// Clear the framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glViewport( 0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

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
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
	RenderUtils::testGLError("Deferred Rendering");
}

// Render all meshes to the shadow buffer
void RenderSystem::renderDirectionalDepthTexture( DirectionalLight *light ) {
	// Get the necessary shaders for this step
	Shader *depth_shader = sm->getShader("depth");
	Shader *skinned_depth_shader = sm->getShader("skinned-depth");

	// Create the view matrix for the light's view
	glm::mat4 light_view_mat = glm::lookAt( shadow_focal_point + light->getLocation(), shadow_focal_point, glm::vec3(0.f,1.f,0.f) );

	// Bind and clear the depth only framebuffer
	((Framebuffer *)light->getDepthFramebuffer())->bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	if( UserSettings::shadow_mode == ShadowMode::ITERATE ) {
		for( int i=0; i<4; i++ ) {
			glViewport( SHADOW_MAP_DIMENSION*i, 0, SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );

			// Bind the shader and render everything for normal meshes
			depth_shader->bind();
			depth_shader->setUniformMat4( "View", light_view_mat );
			depth_shader->setUniformMat4( "Projection", light->getProjectionMatrix(i) );
			drawMeshListVerticesOnly( depth_shader );
			
			// Bind the shader and render everything for skinned meshes
			skinned_depth_shader->bind();
			skinned_depth_shader->setUniformMat4( "View", light_view_mat );
			skinned_depth_shader->setUniformMat4( "Projection", light->getProjectionMatrix(i) );
			drawSkinnedMeshListVerticesOnly( skinned_depth_shader );
		}
	}
	else {
		glViewport( SHADOW_MAP_DIMENSION*2, 0, SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
		// Bind the shader and render everything for normal meshes
		depth_shader->bind();
		depth_shader->setUniformMat4( "View", light_view_mat );
		depth_shader->setUniformMat4( "Projection", light->getProjectionMatrix(2) );
		drawMeshListVerticesOnly( depth_shader );
		
		// Bind the shader and render everything for skinned meshes
		skinned_depth_shader->bind();
		skinned_depth_shader->setUniformMat4( "View", light_view_mat );
		skinned_depth_shader->setUniformMat4( "Projection", light->getProjectionMatrix(2) );
		drawSkinnedMeshListVerticesOnly( skinned_depth_shader );
	}

	// End Render
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
	RenderUtils::testGLError("Depth Texture render");
}

// Create the shadow map texture to pass to the shading step
void RenderSystem::createDirectionalShadowMap( DirectionalLight *light ) {
	// Get the necessary shaders for this step
	Shader *mapping_shader = sm->getShader("directional-shadows");
	mapping_shader->bind();

	// Create the view and projection matrices for the light's view
	glm::mat4 light_view_mat = glm::lookAt( shadow_focal_point + light->getLocation(), shadow_focal_point, glm::vec3(0.f,1.f,0.f) );

	// Bind and clear the mapping buffer
	((Framebuffer *)light->getShadowFramebuffer())->bind();
	glClear( GL_COLOR_BUFFER_BIT );
	glViewport( 0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	// Bind the necessary textures for shadow mapping
	deferred_buffer.getTexture( "position" )->bind( GL_TEXTURE0 );
	deferred_buffer.getTexture( "normal" )->bind( GL_TEXTURE1 );
	((Framebuffer *)light->getDepthFramebuffer())->getDepthTexture()->bind( GL_TEXTURE2 );

	// Load the lightspace transform matrices
	mapping_shader->setUniformMat4( "lightView", light_view_mat );
	for( int i=0; i<4; i++ ) {
		mapping_shader->setUniformMat4( "lightProjections[" + std::to_string(i) + "]", light->getProjectionMatrix(i) );
		mapping_shader->setUniformFloat( "lightDistanceThresholds[" + std::to_string(i) + "]", light->getDistanceThreshold(i) );
	}
	mapping_shader->setUniformVec3( "lightLocation", light->getLocation() );
	mapping_shader->setUniformVec3( "cameraLocation", shadow_focal_point );
	mapping_shader->setUniformFloat( "iterate", UserSettings::shadow_mode == ShadowMode::ITERATE ? 1.f : 0.f );

	// Render the shadow map
	RenderUtils::drawQuad();

	// End Render
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
	RenderUtils::testGLError("Shadow Map render");
}

// Render all meshes to the shadow buffer
void RenderSystem::renderDirectionalVarianceDepthTexture( DirectionalLight *light ) {
	// Get the necessary shaders for this step
	Shader *depth_shader = sm->getShader("depth");
	Shader *skinned_depth_shader = sm->getShader("skinned-depth");

	// Create the view matricx for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->getLocation(), camera_loc, glm::vec3(0.f,1.f,0.f) );
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
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
	RenderUtils::testGLError("Variance Depth Texture render");
}

// Create the shadow map texture to pass to the shading step
void RenderSystem::createDirectionalVarianceShadowMap( DirectionalLight *light ) {
	// Blurs the depth texture from the variance_depth_shadow_buffer Framebuffer
	// The resulting blurred texture is "blurred_depth" in the variance_blurred_depth_out Framebuffer
	variance_blur_process->apply();

	// Get the necessary shaders for this step
	Shader *mapping_shader = sm->getShader("variance-shadows");

	// Create the view and projection matrices for the light's view
	glm::mat4 light_view_mat = glm::lookAt( camera_loc + light->getLocation(), camera_loc, glm::vec3(0.f,1.f,0.f) );
	glm::mat4 variance_sun_proj_mat = glm::ortho( -variance_sun_distances[2], variance_sun_distances[2], -variance_sun_distances[2], variance_sun_distances[2], -100.f, 100.f );

	// Bind and clear the mapping buffer
	shadow_mapping_buffer.bind();
	glClear( GL_COLOR_BUFFER_BIT );
	glViewport( 0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );

	mapping_shader->bind();

	// Bind the necessary textures for shadow mapping
	deferred_buffer.getTexture( "position" )->bind( GL_TEXTURE0 );
	deferred_buffer.getTexture( "normal" )->bind( GL_TEXTURE1 );
	variance_blurred_depth_out.getTexture( "blurred_depth" )->bind( GL_TEXTURE2 );
	
	// Load the lightspace transform matrices
	mapping_shader->setUniformMat4( "lightView", light_view_mat );
	mapping_shader->setUniformMat4( "lightProjection", variance_sun_proj_mat );
	mapping_shader->setUniformVec3( "lightLocation", light->getLocation() );

	// Render the shadow map
	RenderUtils::drawQuad();

	// End Render
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
	RenderUtils::testGLError("Variance Shadow Map render");
}

void RenderSystem::renderPointDepthTextures( PointLight *light ) {

}

void RenderSystem::createPointShadowMap( PointLight *light ) {

}

void RenderSystem::createShadowMaps() {
	for( size_t i = 0; i < directional_lights.size(); i++ ) {
		renderDirectionalDepthTexture( directional_lights[i] );
	}
	for( size_t i = 0; i < directional_lights.size(); i++ ) {
		createDirectionalShadowMap( directional_lights[i] );
	}
}

void RenderSystem::shadingStep() {
	// Bind the shading framebuffer
	shading_buffer.bind();

	glViewport( 0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	Shader *cartoon_shading = sm->getShader("cartoon");
	cartoon_shading->bind();

	deferred_buffer.getTexture( "position" )->bind( GL_TEXTURE0 );
	deferred_buffer.getTexture( "normal" )->bind( GL_TEXTURE1 );
	deferred_buffer.getTexture( "diffuse" )->bind( GL_TEXTURE2 );
	deferred_buffer.getTexture( "emissive" )->bind( GL_TEXTURE3 );
	if( directional_lights.size() > 0 )
		((Framebuffer *)directional_lights[0]->getShadowFramebuffer())->getTexture( "shadow_map" )->bind( GL_TEXTURE4 );
	
	cartoon_shading->setUniformVec3( "cameraLoc", camera_loc );

	cartoon_shading->setUniformFloat("ambientAmount", 0.3f );

	cartoon_shading->setUniformFloat( "applyShadows", UserSettings::shadow_mode == ShadowMode::NONE ? 0.f : 1.f );

	if( directional_lights.size() > 0 ) {
		cartoon_shading->setUniformVec3( "light.location", directional_lights[0]->getLocation() );
		cartoon_shading->setUniformVec3( "light.diffuse", directional_lights[0]->getDiffuse() );
		cartoon_shading->setUniformVec3( "light.specular", directional_lights[0]->getSpecular() );
	}
	cartoon_shading->setUniformFloat( "light.linearAttenuation", 0.f );
	cartoon_shading->setUniformFloat( "light.quadraticAttenuation", 0.f );
	cartoon_shading->setUniformFloat( "light.directional", 1.f );

	RenderUtils::drawQuad();

	RenderUtils::testGLError("Shading");
	
	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
}

// New shading step
// Applies all light contributions in individual passes, performing additive blending
void RenderSystem::performShading() {
	shading_buffer.bind();
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClear( GL_COLOR_BUFFER_BIT );

	// Enable additive blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );

	// Send the emissive texture to the BrightColor texture
	applyEmissive();
	
	// Apply all directonal light contributions
	for( DirectionalLight *light : directional_lights ) {
		applyDirectionalLight( light );
	}

	// Apply all point light contributions
	for( PointLight *light : point_lights ) {
		applyPointLight( light );
	}

	// Finish up
	glDisable( GL_BLEND );
	RenderUtils::testGLError( "performShading()" );
}

/**
 * Simply applies the emissive texture from the deferred rendering step to the BrightColor target of the shading buffer
 * ASSUMPTIONS:
 * 	   The correct render target (shading_buffer) is already bound
 * 	   Additive blending is already enabled
 */
void RenderSystem::applyEmissive() {
	Shader *emissive_shader = sm->getShader("emissive");
	emissive_shader->bind();
	deferred_buffer.getTexture( "emissive" )->bind( GL_TEXTURE0 );
	RenderUtils::drawQuad();
	RenderUtils::testGLError( "applyEmissive()" );
}

/**
 *  Applies the supplied light's contribution to the output texture
 *  ASSUMPTIONS:
 * 	   The correct render target (shading_buffer) is already bound
 * 	   Additive blending is already enabled
 */
void RenderSystem::applyDirectionalLight( DirectionalLight *light ) {
	Shader *dir_light_shader = sm->getShader("directional-light");
	dir_light_shader->bind();
	dir_light_shader->setUniformVec3( "cameraLoc", camera_loc );

	// Bind appropriate textures
	// Uniform locations already set in setupShaders()
	deferred_buffer.getTexture( "position" )->bind( GL_TEXTURE0 );
	deferred_buffer.getTexture( "normal" )->bind( GL_TEXTURE1 );
	deferred_buffer.getTexture( "diffuse" )->bind( GL_TEXTURE2 );
	((Framebuffer *)light->getShadowFramebuffer())->getTexture( "shadow_map" )->bind( GL_TEXTURE3 );

	// Set uniform light information
	dir_light_shader->setUniformVec3( "light.location", light->getLocation() );
	dir_light_shader->setUniformVec3( "light.ambient", light->getAmbient() );
	dir_light_shader->setUniformVec3( "light.diffuse", light->getDiffuse() );
	dir_light_shader->setUniformVec3( "light.specular", light->getSpecular() );

	// Render
	RenderUtils::drawQuad();

	RenderUtils::testGLError( ("Applying directional light " + light->getID()).c_str() );
}

/**
 * Applies the supplied light's contribution to the output texture
 *  ASSUMPTIONS:
 * 	   The correct render target (shading_buffer) is already bound
 * 	   Additive blending is already enabled
 */
void RenderSystem::applyPointLight( PointLight *light ) {
	Shader *point_light_shader = sm->getShader("point-light");
	point_light_shader->bind();
	point_light_shader->setUniformVec3( "cameraLoc", camera_loc );

	// Bind appropriate textures
	// Uniform locations already set in setupShaders()
	deferred_buffer.getTexture( "position" )->bind( GL_TEXTURE0 );
	deferred_buffer.getTexture( "normal" )->bind( GL_TEXTURE1 );
	deferred_buffer.getTexture( "diffuse" )->bind( GL_TEXTURE2 );
	// ((Framebuffer *)light->getShadowFramebuffer())->getTexture( "shadow_map" )->bind( GL_TEXTURE3 );

	// Set uniform light information
	point_light_shader->setUniformVec3( "light.location", light->getLocation() );
	point_light_shader->setUniformVec3( "light.ambient", light->getAmbient() );
	point_light_shader->setUniformVec3( "light.diffuse", light->getDiffuse() );
	point_light_shader->setUniformVec3( "light.specular", light->getSpecular() );
	point_light_shader->setUniformFloat( "light.linearAttenuation", light->getLinearAttenuation() );
	point_light_shader->setUniformFloat( "light.quadraticAttenuation", light->getQuadraticAttenuation() );

	// Render
	RenderUtils::drawQuad();

	RenderUtils::testGLError( ("Applying point light " + light->getID()).c_str() );
}

/**
 * Renders all overlay elements at the final stage
 * CURRENT ASSUMPTIONS (may change)
 * 	  The desired framebuffer and viewport size are already bound and set
 * 		(currently, this is the composite framebuffer as this is done after the shading step)
 */
void RenderSystem::renderOverlay() {
	Shader * overlay_shader = sm->getShader("overlay");
	overlay_shader->bind();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	drawOverlayMeshList( true, overlay_shader );
	
	glDisable( GL_BLEND );

	RenderUtils::testGLError("Overlay");

	// glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	// glUseProgram(0);
}

/**
 * Performs any final corrections, (currently HDR and gamma) and renders to the screen
 */
void RenderSystem::correctAndRenderFinal() {
	Shader *correction_shader = sm->getShader("hdr-gamma");
	correction_shader->bind();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, RenderUtils::getViewWidth(), RenderUtils::getViewHeight() );
	correction_shader->setUniformFloat( "gamma", 2.2 );
	correction_shader->setUniformFloat( "exposure", 1.0 );
	correction_shader->setUniformFloat( "useExposure", float(UserSettings::use_exposure) );

	composite_buffer.getTexture( "composite_texture" )->bind( GL_TEXTURE0 );
	RenderUtils::drawQuad();

	// drawTexture( composite_buffer.getTexture( "composite_texture" )->getID() );

	RenderUtils::testGLError("HDR/Gamma");
}



/***************************************
 *    RENDERSYSTEM CONFIG FUCNTIONS    *
 ***************************************/


/**
 * Sets the camera to be used for rendering 3D meshes and updates any relevant variables
 */
void RenderSystem::registerCamera( Camera *to_register ) { 
	camera = to_register; 
	if(camera != nullptr) {
		camera_loc = glm::vec3(camera->getEyePos());
		mesh_list->setCameraLoc(camera_loc);
		skinned_mesh_list->setCameraLoc(camera_loc);
	}
}

/**
 * Add a directional light to the RenderSystem
 * Creates framebuffers and necessary textures for creating a shadow map for the light
 */
void RenderSystem::addDirectionalLight( DirectionalLight *new_light ) {
	directional_lights.push_back( new_light );

	Framebuffer *new_depth_fb = new Framebuffer();
	new_depth_fb->addDepthBuffer( 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	new_depth_fb->addDepthTexture( "depth", 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	new_light->setDepthFramebuffer( (void *)new_depth_fb );

	Framebuffer *new_shadow_fb = new Framebuffer();
	new_shadow_fb->addColorTexture( "shadow_map", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	new_light->setShadowFramebuffer( (void *)new_shadow_fb );
}

/**
 * Removes the indicated directional light and deletes the framebuffers created when the light was registered
 */
void RenderSystem::removeDirectionalLight( DirectionalLight *to_remove ) {
	// Make sure the light is registered before trying to remove it and delete its buffers
	auto it = std::remove( directional_lights.begin(), directional_lights.end(), to_remove );
	if( it != directional_lights.end() ) {
		delete ((Framebuffer*) to_remove->getDepthFramebuffer());
		delete ((Framebuffer*) to_remove->getShadowFramebuffer());
		directional_lights.erase( it );
	}
}

/**
 * Remove all directional lights from the render system
 */
void RenderSystem::clearDirectionalLights() {
	for( DirectionalLight *light : directional_lights ) {
		removeDirectionalLight( light );
	}
}

/**
 * Add a point light to the RenderSystem
 * Creates framebuffers and necessary textures for creating a shadow map for the light
 */
void RenderSystem::addPointLight( PointLight *new_light ) {
	point_lights.push_back( new_light );

	// TODO
	// Framebuffer *new_depth_fb = new Framebuffer();
	// new_depth_fb->addDepthBuffer( 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	// new_depth_fb->addDepthTexture( "depth", 4 * SHADOW_MAP_DIMENSION, SHADOW_MAP_DIMENSION );
	// new_light->setDepthFramebuffer( (void *)new_depth_fb );

	// Framebuffer *new_shadow_fb = new Framebuffer();
	// new_shadow_fb->addColorTexture( "shadow_map", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight() );
	// new_light->setShadowFramebuffer( (void *)new_shadow_fb );
}

/**
 * Removes the indicated point light and deletes the framebuffers created when the light was registered
 */
void RenderSystem::removePointLight( PointLight *to_remove ) {
	// Make sure the light is registered before trying to remove it and delete its buffers
	auto it = std::remove( point_lights.begin(), point_lights.end(), to_remove );
	if( it != point_lights.end() ) {
		point_lights.erase( it );
		// delete ((Framebuffer*) to_remove->getDepthFramebuffer());
		// delete ((Framebuffer*) to_remove->getShadowFramebuffer());
	}
}

/**
 * Removes all point lights from the render system
 */
void RenderSystem::clearPointLights() {
	for( PointLight *light : point_lights ) {
		removePointLight( light );
	}
}
