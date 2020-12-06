#ifndef MATERIALS_H
#define MATERIALS_H

#include <GL/glew.h>

#include "../../Utilities/FilesystemWrapper.hpp"
#include "Shader.hpp"

class Material {
  private:
    GLuint diffuse = 0;
    GLuint emissive = 0;
    float shininess = 0;

  public:
    Material(GLuint texture_, GLuint emissive_, float shininess_);

    /** 
     * Binds the material to the given shader
     * @param shader shader to bind to
     * @param use_shininess defaults to true 
     */
    void bind(Shader *shader, bool use_shininess = true);

    /** @return the diffuse texture */
    inline GLuint getDiffuse() { return diffuse; }
    /** @return the emissive texture */
    inline GLuint getEmissive() { return emissive; }
    /** @return the shininess value */
    inline float getShininess() { return shininess; }
};

class MaterialFactory {
  private:
    static GLuint default_diffuse;
    static GLuint default_emissive;
    static float default_shininess;

    static Material *default_material;
    static Material *default_material_2d;

    static GLuint loadMaterialTexture(fs::path path, fs::path textures_folder, bool clamp);
    static float loadMaterialValue(fs::path path);

  public:
    static Material *createMaterial(fs::path input_directory);
    static Material *createMaterial2D(fs::path input_directory);
    static Material *createMaterial(fs::path input_directory, fs::path textures_path, bool clamp_textures);

    static void loadDefaults();
    static Material *getDefaultMaterial() { return default_material; }
    static Material *getDefaultMaterial2D() { return default_material_2d; }
};

#endif