#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderUtils.hpp"
#include "Renderable.hpp"

#include "ShaderManager.hpp"
#include "Framebuffer.hpp"
#include "Skybox.hpp"
#include "../../GameObject.hpp"
#include "../../GameObjects/Camera.hpp"
#include "../../GameObjects/PointLight.hpp"
#include "../../GameObjects/DirectionalLight.hpp"
#include "../../SettingsManager.hpp"
#include "PostProcessing/FXAA.hpp"
#include "PostProcessing/VolumetricLightScattering.hpp"
#include "PostProcessing/Bloom.hpp"
#include "PostProcessing/Blur.hpp"

#include "MeshList/MeshList.hpp"

class RenderSystem {
private:
    // Keeping for now for the old shading step
    DirectionalLight *old_sun;

    Camera *camera;

    // Temporary VAO to render everything for now
    GLuint BASE_VAO;


    // The deferred rendering framebuffer
    Framebuffer deferred_buffer;
    Framebuffer shading_buffer;
    
    // Framebuffers and processes required for basic shadow mapping
    Framebuffer depth_shadow_buffer;
    Framebuffer shadow_blurred_depth_out; // To be used as the out buffer from a blur PostProcess

    // Framebuffers and processes required for variance shadow mapping
    Framebuffer variance_depth_shadow_buffer;
    Framebuffer variance_blurred_depth_out; // To be used as the out buffer from a blur PostProcess
    Blur * variance_blur_process; // Blurs the depth

    // Framebuffer and process for creating the final shadow map, regardless of technique used
    Framebuffer shadow_mapping_buffer;

    // Framebuffer for the final output before gamma correction and HDR
    Framebuffer composite_buffer;

    // The shader manager for the render system
    ShaderManager *sm;

    // Model, View, and Projection matrices for the program
    glm::mat4 view_mat, proj_mat;
    glm::vec3 camera_loc;
    glm::vec3 shadow_focal_point;

    // skybox data
    Skybox * skybox;

    // The active lights in the scene
    std::vector<DirectionalLight*> directional_lights;
    std::vector<PointLight*> point_lights;

    // vectors for lists of different types of geometry. This will be used to optimize the rendering pipeline by reducing 
    // how often the shader is switched during rendering
    
    /**
            MeshList and sorted boolean will likely change with material overhaul. the process for drawing the meshlists will 
            also drastically change as shaders will be moved to materials. This will allow for a more streamlined rendering 
            process and will reduce the amount of code within rendersystem
    */

    MeshList * mesh_list;
    MeshList * skinned_mesh_list;
    MeshList * overlay_mesh_list;

    bool sorted;

    // Post processes
    FXAA * FXAA_process;
    VolumetricLightScattering * vls_post_process;
    Bloom * bloom_post_process;

    /**
        Rendering Pipeline Setup
    **/

    // Set up the framebuffers necessary for the rendering process
    void addFramebufferTextures();


    /**
        Rendering Pipeline Util functions
         - functions that are used multiple times throughout the rendering pipeline
    **/

    // Draw a quad with texture tex that takes up the whole viewport
    void drawTexture( GLuint tex );


    // draws the meshList
    void drawMeshList(bool useMaterials, Shader * shader);
    void drawSkinnedMeshList(bool useMaterials, Shader * shader);
    void drawOverlayMeshList(bool useMaterials, Shader * shader);

    void drawMeshListVerticesOnly(Shader * shader);
    void drawSkinnedMeshListVerticesOnly(Shader * shader);


    /**
        Rendering pipeline
    **/

    // Call in the constructor to set uniforms in shaders which will not change (texture units)
    void setupShaders();

    // Create the necessary matrices for rendering
    void createOrthoMatrices();

    // Do the deferred rendering step
    void deferredRenderStep();

    // shadows
    void renderDirectionalDepthTexture( DirectionalLight *light );
    void createDirectionalShadowMap( DirectionalLight *light );

    void renderDirectionalVarianceDepthTexture( DirectionalLight *light );
    void createDirectionalVarianceShadowMap( DirectionalLight *light );

    void drawDepthTexture( GLuint tex );

    // NEW
    // Create shadow maps for all active lights
    void createShadowMaps();

    void renderPointDepthTextures( PointLight * light );
    void createPointShadowMap( PointLight *light );

    // shading step
    void shadingStep();

    // NEW shading step
    void performShading();

    // Sub methods of performShading()
    void applyEmissive();
    void applyDirectionalLight( DirectionalLight *light );
    void applyPointLight( PointLight *light );

    // 2D overlay elements
    void renderOverlay();

    // Perform gamma and HDR corrections and render to the screen
    void correctAndRenderFinal();

    // Singleton constructor
    RenderSystem();

    // destructor
    ~RenderSystem();

public:
    // Get the singleton instance
    static RenderSystem & getRenderSystem();

    // Set up render lists - iterate through the scenegraph and identify what needs to be drawn
    void populateRenderLists(GameObject * scenegraph);
    // non recursive - adds single node
    void addToRenderLists( GameObject * game_object );

    void clearRenderLists();
    void removeGameObjectFromRenderLists( GameObject * game_object );
    void removeGameObjectFromRenderListsRecursive( GameObject * game_object );

    // Primary render function
    // Encapsulates all rendering done into a single function to call from a state
    void render( double dt );

    // To be called if the display resolution changes
    // May be moved in the future, but currently sets RenderUtils::view_width/view_height
    //  and modifies the overlay projection matrix accordingly
    void reshape( int x_size, int y_size );

    // To be called if rendering resolution changes
    // Recreates framebuffers and textures with newly specified UserSettings::texture_width/texture_height
    void recreateFramebuffers();

    // Set the camera for the rendersystem
    void registerCamera( Camera *to_register );
    inline Camera * getRegisteredCamera() { return camera; }
    
    // Light management
    // Any added lights are assumed currently active
    void addDirectionalLight( DirectionalLight *new_light );
    void removeDirectionalLight( DirectionalLight *to_remove );
    void clearDirectionalLights();

    void addPointLight( PointLight *new_light );
    void removePointLight( PointLight *to_remove );
    void clearPointLights();

    inline void setSkybox(Skybox * skybox_in) { skybox = skybox_in; }
};


#endif
