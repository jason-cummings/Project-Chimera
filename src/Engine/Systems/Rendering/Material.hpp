#ifndef MATERIALS_H
#define MATERIALS_H

#include <GL/glew.h>

#include "Shader.hpp"
#include "../../Utilities/FilesystemWrapper.hpp"

class Material
{
private:   
    GLuint texture = 0;
    GLuint emissive = 0;
    float shininess = 0;

    static Material *default_material;
    static Material *default_material_2d;

public:
    Material(GLuint texture_, GLuint emissive_, float shininess_);
    void bind(Shader *shader, bool use_shininess = true);
    inline GLuint getTexture() { return texture; }
    inline GLuint getEmissive() { return emissive; }
    inline float getShininess() { return shininess; }

    static void loadDefaultMaterial();
    static Material * getDefaultMaterial() { return default_material; }
    static Material * getDefaultMaterial2D() { return default_material_2d; }
};

class MaterialFactory {
public:
    static Material* createMaterial(fs::path input_directory);
    static Material* createMaterial2D(fs::path input_directory);
    static Material* createMaterial(fs::path input_directory, fs::path textures_path, bool clamp_textures);
};

#endif