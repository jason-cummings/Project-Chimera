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
#include "PostProcessing/Blur.hpp"

#include "MeshList/MeshList.hpp"
#include "MeshList/NoSortMeshList.hpp"
#include "MeshList/FrontToBackMeshList.hpp"
#include "MeshList/BackToFrontMeshList.hpp"


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
    
    // Framebuffers and processes required for basic shadow mapping
    Framebuffer depth_shadow_buffer;
    Framebuffer shadow_blurred_depth_out; // To be used as the out buffer from a blur PostProcess

    // Framebuffers and processes required for variance shadow mapping
    Framebuffer variance_depth_shadow_buffer;
    Framebuffer variance_blurred_depth_out; // To be used as the out buffer from a blur PostProcess
    Blur * variance_blur_process; // Blurs the depth

    // Framebuffer and process for creating the final shadow map, regardless of technique used
    Framebuffer shadow_mapping_buffer;
    Blur * shadow_map_blur_process;

    // The shader manager for the render system
    ShaderManager *sm;

    // Model, View, and Projection matrices for the program
    glm::mat4 view_mat, proj_mat;
    glm::vec3 camera_loc;

    // skybox data
    Skybox * skybox;

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

    // 2D overlay elements
    void renderOverlay();

    // Singleton constructor
    RenderSystem();

    // destructor
    ~RenderSystem();

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
    void registerCamera( Camera *to_register );
    inline Camera * getRegisteredCamera() { return camera; }
    
    inline void setSkybox(Skybox * skybox_in) { skybox = skybox_in; }
};


#endif
