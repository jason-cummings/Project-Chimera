#ifndef MATERIALS_H
#define MATERIALS_H

#include <GL/glew.h>
#include "Shader.hpp"
#include "TextureLoader.hpp"

// Path to default material from Asset
#define DEFAULT_MATERIAl_PATH "Materials/Missing"
#define DEFAULT_TEXTURES_PATH "Textures"

class Material
{
private:   
    GLuint texture = 0;
    GLuint emissive = 0;
    float shininess = 0;

    static Material *default_material;

public:
    Material(GLuint texture_, GLuint emissive_, float shininess_);
    void bind(Shader *shader, bool use_shininess = true);
    inline GLuint getTexture() { return texture; }
    inline GLuint getEmissive() { return emissive; }
    inline float getShininess() { return shininess; }

    static void loadDefaultMaterial();
    static Material * getDefaultMaterial() { return default_material; }
};

class MaterialFactory {
public:
    static Material* createMaterial(fs::path input_directory);
    static Material* createMaterial(fs::path input_directory, fs::path textures_path);
};

#endif