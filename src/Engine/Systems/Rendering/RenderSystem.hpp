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
#include "../../SettingsManager.hpp"
#include "PostProcessing/FXAA.hpp"
#include "PostProcessing/VolumetricLightScattering.hpp"
#include "PostProcessing/Bloom.hpp"



struct Light {
    glm::vec3 location;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float linear_attenuation;
    float quadratic_attenuation;
    float directional;
};

class RenderSystem {
private:
    Camera *camera;

    // A hard coded light to act as the sun
    Light sun;
    glm::mat4 sun_proj_mats[4];

    // Temporary VAO to render everything for now
    GLuint BASE_VAO;


    // The deferred rendering framebuffer
    Framebuffer deferred_buffer;
    Framebuffer shading_buffer;
    
    // Blur buffers and settings
    bool use_bloom;

    // Volumetric light scattering settings

    // The depth-only framebuffer for shadows and buffer for shadow mapping
    Framebuffer depth_shadow_buffer;
    Framebuffer shadow_mapping_buffer;

    Framebuffer variance_depth_shadow_buffer;
    Framebuffer variance_shadow_blur_buffer[2];

    // Variables for the output sizes of the textures
    int texture_width, texture_height; // Render resolution
    int view_width, view_height; // Actual display

    // The shader manager for the render system
    ShaderManager *sm;

    // Model, View, and Projection matrices for the program
    glm::mat4 view_mat, proj_mat;
    glm::vec3 camera_loc;

    // skybox data
    Skybox * skybox;

    // vectors for lists of different types of geometry. This will be used to optimize the rendering pipeline by reducing 
    // how often the shader is switched during rendering
    std::vector<GameObject*> mesh_list;
    std::vector<GameObject*> skinned_mesh_list;
    std::vector<GameObject*> overlay_mesh_list;

    // Post processes
    PostProcess * FXAA_process;
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

    // Create the necessary matrices for rendering
    void createOrthoMatrices();

    // Do the deferred rendering step
    void deferredRenderStep();

    // shadows
    void renderDirectionalDepthTexture( Light *light );
    void createDirectionalShadowMap( Light *light );

    void renderVarianceDirectionalDepthTexture( Light *light );
    void createDirectionalVarianceShadowMap( Light *light );

    void drawDepthTexture( GLuint tex );

    // shading step
    void shadingStep();

    // bloom
    // void applyBloom();

    // volumetric light scattering
    // void applyVolumetricLightScattering();

    // 2D overlay elements
    void renderOverlay();


    RenderSystem();

public:

    // Set up render lists - iterate through the scenegraph and identify what needs to be drawn
    void populateRenderLists(GameObject * scenegraph);
    // non recursive - adds single node
    void addToRenderLists( GameObject * game_object );

    void clearRenderLists();
    void removeGameObjectFromRenderLists( GameObject * game_object );
    void removeGameObjectFromRenderListsRecursive( GameObject * game_object );

    // Get the singleton instance
    static RenderSystem & getRenderSystem();

    // RENDER
    void render( double dt );

    void reshape( int x_size, int y_size );

    void recreateFramebuffers();

    // Set the camera for the rendersystem
    inline void registerCamera( Camera *to_register ) { camera = to_register; }
    inline Camera * getRegisteredCamera() { return camera; }
    
    inline void setSkybox(Skybox * skybox_in) { skybox = skybox_in; }

    inline int getViewWidth() { return view_width; }
    inline int getViewHeight() { return view_height; }
};


#endif
