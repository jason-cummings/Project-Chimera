#include "ShaderManager.hpp"

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

    Shader *cartoon_shading = new Shader("cartoon", "CartoonShading.vert", "CartoonShading.frag");
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
    shaders["cartoon"] = cartoon_shading;

    Shader *overlay_shader = new Shader( "overlay", "OverlayShading.vert", "OverlayShading.frag" );
    overlay_shader->addUniform( "Model" );
    overlay_shader->addUniform( "View" );
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
    directional_shadows_shader->addUniform( "lightView" );
    directional_shadows_shader->addUniform( "lightProjection" );
    directional_shadows_shader->addUniform( "lightLocation" );
    shaders["directional-shadows"] = directional_shadows_shader;
}

Shader * ShaderManager::getShader( std::string shader_name ) {
    return shaders[shader_name];
}

void ShaderManager::bindShader( std::string shader_name ) {
    shaders[shader_name]->bind();
}
