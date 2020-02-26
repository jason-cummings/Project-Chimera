#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "Shader.hpp"

class ShaderManager {
private:
    std::map<std::string, Shader*> shaders;

    // Singleton instance
    static ShaderManager *sm;

    ShaderManager();
public:
    // Create ShaderManager instance if necessary and return
    static ShaderManager * getShaderManager();

    void loadShaders();

    Shader *getShader( std::string shader_name );

    void bindShader( std::string shader_name );
};

#endif
