#ifndef MATERIAL_FACTORY_H
#define MATERIAL_FACTORY_H

#include <GL/glew.h>
#include <fstream>
#include <string>
#include "Material.hpp"
#include "TextureLoader.hpp"

class MaterialFactory{
public:
    static Material* createMaterial(fs::path input_directory, fs::path textures_path);
private:

};

#endif