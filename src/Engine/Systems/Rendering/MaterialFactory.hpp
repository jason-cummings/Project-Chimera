#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Material.hpp"

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