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
    Shader *defaultShader = new Shader( "default", "default.vert", "default.frag" );
    defaultShader->addUniform("Model");
    defaultShader->addUniform("View");
    defaultShader->addUniform("Projection");
    shaders["default"] = defaultShader;
}

Shader * ShaderManager::getShader( std::string shader_name) {
    return shaders[shader_name];
}

void ShaderManager::bindShader( std::string shader_name ) {
    shaders[shader_name]->bind();
}
