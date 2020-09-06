#include "ShaderManager.hpp"

#include <iostream>

ShaderManager * ShaderManager::sm = nullptr;

ShaderManager::ShaderManager() {
    loadShaders();
}

ShaderManager * ShaderManager::getShaderManager() {
    // Initialize the singleton instance if necessary
    if( sm == nullptr ) {
        sm = new ShaderManager();
    }
    return sm;
}

void ShaderManager::loadShaders() {
    // The default shader
    Shader *default_shader = new Shader( "default", "Default.vert", "Default.frag" );
    default_shader->addUniform( "Model" );
    default_shader->addUniform( "View" );
    default_shader->addUniform( "Projection" );
    shaders["default"] = default_shader;
    
    // The basic deferred rendering shader
    Shader *basic_deferred_shader = new Shader( "basic-deferred", "BasicDeferredShading.vert", "BasicDeferredShading.frag" );
    basic_deferred_shader->addUniform( "Model" );
    basic_deferred_shader->addUniform( "NormalMatrix" );
    basic_deferred_shader->addUniform( "View" );
    basic_deferred_shader->addUniform( "Projection" );
    basic_deferred_shader->addUniform( "materialShininess" );
    basic_deferred_shader->addUniform( "colorTexture" );
    basic_deferred_shader->addUniform( "emissiveTexture" );
    shaders["basic-deferred"] = basic_deferred_shader;

    Shader *skinned_deferred_shader = new Shader( "skinned-deferred", "SkinnedDeferredShading.vert", "BasicDeferredShading.frag" );
    skinned_deferred_shader->addUniform( "Model" );
    skinned_deferred_shader->addUniform( "NormalMatrix" );
    skinned_deferred_shader->addUniform( "View" );
    skinned_deferred_shader->addUniform( "Projection" );
    skinned_deferred_shader->addUniform( "materialShininess" );
    skinned_deferred_shader->addUniform( "colorTexture" );
    skinned_deferred_shader->addUniform( "emissiveTexture" );

    for(int i = 0; i < 25; i++) {
        skinned_deferred_shader->addUniform("boneMatrices[" + std::to_string(i) + "]");
    }
    shaders["skinned-deferred"] = skinned_deferred_shader;

    Shader *quad_shader = new Shader( "quad", "DrawQuad.vert", "DrawQuad.frag" );
    quad_shader->addUniform( "colorTexture" );
    shaders["quad"] = quad_shader;

    Shader *cartoon_shading = new Shader("cartoon", "DrawQuad.vert", "CartoonShading.frag");
    cartoon_shading->addUniform("positionTexture");
    cartoon_shading->addUniform("normalTexture");
    cartoon_shading->addUniform("diffuseTexture");
    cartoon_shading->addUniform("emissiveTexture");
    cartoon_shading->addUniform("shadowTexture");
    cartoon_shading->addUniform("cameraLoc");
    cartoon_shading->addUniform("light.location");
    cartoon_shading->addUniform("light.diffuse");
    cartoon_shading->addUniform("light.specular");
    cartoon_shading->addUniform("light.linearAttenuation");
    cartoon_shading->addUniform("light.quadraticAttenuation");
    cartoon_shading->addUniform("light.directional");
    cartoon_shading->addUniform("ambientAmount");
    cartoon_shading->addUniform("applyShadows");
    shaders["cartoon"] = cartoon_shading;

    Shader *emissive_shader = new Shader("emissive", "DrawQuad.vert", "ApplyEmissiveShading.frag");
    emissive_shader->addUniform("emissiveTexture");
    shaders["emissive"] = emissive_shader;

    Shader *directional_light_shader = new Shader("directional-light", "DrawQuad.vert", "ApplyDirectionalLightWithShadows.frag");
    directional_light_shader->addUniform("positionTexture");
    directional_light_shader->addUniform("normalTexture");
    directional_light_shader->addUniform("diffuseTexture");
    directional_light_shader->addUniform("shadowTexture");
    directional_light_shader->addUniform("cameraLoc");
    directional_light_shader->addUniform("light.ambient");
    directional_light_shader->addUniform("light.location");
    directional_light_shader->addUniform("light.diffuse");
    directional_light_shader->addUniform("light.specular");
    directional_light_shader->addUniform("shadeCartoon");
    shaders["directional-light"] = directional_light_shader;

    Shader *point_light_shader = new Shader("point-light", "ApplyPointLight.vert", "ApplyPointLight.frag");
    point_light_shader->addUniform("Model");
    point_light_shader->addUniform("View");
    point_light_shader->addUniform("Projection");
    point_light_shader->addUniform("positionTexture");
    point_light_shader->addUniform("normalTexture");
    point_light_shader->addUniform("diffuseTexture");
    // point_light_shader->addUniform("shadowTexture");
    point_light_shader->addUniform("cameraLoc");
    point_light_shader->addUniform("viewportSize");
    point_light_shader->addUniform("light.ambient");
    point_light_shader->addUniform("light.location");
    point_light_shader->addUniform("light.diffuse");
    point_light_shader->addUniform("light.specular");
    point_light_shader->addUniform("light.linearAttenuation");
    point_light_shader->addUniform("light.quadraticAttenuation");
    point_light_shader->addUniform("shadeCartoon");
    shaders["point-light"] = point_light_shader;

    Shader *overlay_shader = new Shader( "overlay", "OverlayShading.vert", "OverlayShading.frag" );
    overlay_shader->addUniform( "Model" );
    overlay_shader->addUniform( "Projection" );
    overlay_shader->addUniform( "colorTexture" );
    overlay_shader->addUniform( "emissiveTexture" );
    shaders["overlay"] = overlay_shader;
    
    Shader *depth_shader = new Shader( "depth", "RenderDepth.vert", "RenderDepth.frag" );
    depth_shader->addUniform( "Model" );
    depth_shader->addUniform( "View" );
    depth_shader->addUniform( "Projection" );
    shaders["depth"] = depth_shader;

    Shader *skinned_depth_shader = new Shader( "skinned-depth", "RenderSkinnedDepth.vert", "RenderDepth.frag" );
    skinned_depth_shader->addUniform( "Model" );
    skinned_depth_shader->addUniform( "View" );
    skinned_depth_shader->addUniform( "Projection" );
    for(int i = 0; i < 25; i++) {
        skinned_depth_shader->addUniform("boneMatrices[" + std::to_string(i) + "]");
    }
    shaders["skinned-depth"] = skinned_depth_shader;

    Shader *depth_tex_shader = new Shader( "draw-depth-tex", "DrawQuad.vert", "DrawDepthTex.frag" );
    depth_tex_shader->addUniform( "depthMap" );
    shaders["draw-depth-tex"] = depth_tex_shader;
    
    Shader *directional_shadows_shader = new Shader( "directional-shadows", "DrawQuad.vert", "CreateDirectionalShadowMap.frag" );
    directional_shadows_shader->addUniform( "positionTexture" );
    directional_shadows_shader->addUniform( "normalTexture" );
    directional_shadows_shader->addUniform( "depthTexture" );
    directional_shadows_shader->addUniform( "lightLocation" );
    directional_shadows_shader->addUniform( "lightView" );
    for( int i=0; i<4; i++ ) {
        directional_shadows_shader->addUniform( "lightProjections[" + std::to_string(i) + "]" );
        directional_shadows_shader->addUniform( "lightDistanceThresholds[" + std::to_string(i) + "]" );
    }
    directional_shadows_shader->addUniform( "cameraLocation" );
    directional_shadows_shader->addUniform( "iterate" );
    shaders["directional-shadows"] = directional_shadows_shader;
    
    Shader *variance_shadows_shader = new Shader( "variance-shadows", "DrawQuad.vert", "CreateDirectionalVarianceShadowMap.frag" );
    variance_shadows_shader->addUniform( "positionTexture" );
    variance_shadows_shader->addUniform( "normalTexture" );
    variance_shadows_shader->addUniform( "depthTexture" );
    variance_shadows_shader->addUniform( "lightLocation" );
    variance_shadows_shader->addUniform( "lightView" );
    variance_shadows_shader->addUniform( "lightProjection" );
    shaders["variance-shadows"] = variance_shadows_shader;
    
    Shader *skybox_shader = new Shader("skybox", "SkyboxDeferred.vert", "SkyboxDeferred.frag");
    skybox_shader->addUniform( "View" );
    skybox_shader->addUniform( "Projection" );
    skybox_shader->addUniform( "skybox" );
    skybox_shader->addUniform( "toLight" );
    skybox_shader->addUniform( "threshold" );
    shaders["skybox"] = skybox_shader;
    
    Shader *blur_shader = new Shader("blur", "DrawQuad.vert", "GaussianBlur.frag");
    blur_shader->addUniform( "horizontal" );
    blur_shader->addUniform( "colorTexture" );
    shaders["blur"] = blur_shader;

    Shader *linear_blur_shader = new Shader("linear-blur", "DrawQuad.vert", "GaussianBlurLinear.frag");
    linear_blur_shader->addUniform( "horizontal" );
    linear_blur_shader->addUniform( "colorTexture" );
    shaders["linear-blur"] = linear_blur_shader;

    Shader *mean_blur_shader = new Shader("mean-blur", "DrawQuad.vert", "MeanBlur.frag");
    mean_blur_shader->addUniform( "horizontal" );
    mean_blur_shader->addUniform( "colorTexture" );
    shaders["mean-blur"] = mean_blur_shader;

    Shader * vls_shader = new Shader("volumetricLightScattering","DrawQuad.vert","VolumetricLightScattering.frag");
    vls_shader->addUniform("sunScreenCoords");
    vls_shader->addUniform("frame");
    shaders["volumetricLightScattering"] = vls_shader;

    Shader *FXAA_shader = new Shader("FXAA", "DrawQuad.vert", "FXAA.frag");
    FXAA_shader->addUniform( "position_texture" );
    FXAA_shader->addUniform( "color_texture" );
    shaders["FXAA"] = FXAA_shader;
    
    Shader *text_shader = new Shader( "text", "OverlayShading.vert", "CreateTextTexture.frag" );
    text_shader->addUniform( "Model" );
    text_shader->addUniform( "Projection" );
    text_shader->addUniform( "textColor" );
    text_shader->addUniform( "textTexture" );
    shaders["text"] = text_shader;

    Shader *hdr_gamma_shader = new Shader( "hdr-gamma", "DrawQuad.vert", "HDRandGamma.frag" );
    hdr_gamma_shader->addUniform( "colorTexture" );
    hdr_gamma_shader->addUniform( "gamma" );
    hdr_gamma_shader->addUniform( "exposure" );
    hdr_gamma_shader->addUniform( "useExposure" );
    shaders["hdr-gamma"] = hdr_gamma_shader;
}

Shader * ShaderManager::getShader( std::string shader_name ) {
    return shaders[shader_name];
}

void ShaderManager::bindShader( std::string shader_name ) {
    shaders[shader_name]->bind();
}
