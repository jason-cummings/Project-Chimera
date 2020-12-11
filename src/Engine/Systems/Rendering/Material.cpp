#include "Material.hpp"

#include <iostream>

#include "../../Utilities/Asset.hpp"
#include "TextureLoader.hpp"

// Path to default material from Asset
#define DEFAULT_MATERIAL_PATH "Materials/Missing"
#define DEFAULT_TEXTURES_PATH "Textures"

#define DIFFUSE_DEFINITION_FILENAME "diffuse.txt"
#define EMISSIVE_DEFINITION_FILENAME "emissive.txt"
#define SHININESS_DEFINITION_FILENAME "shininess"

// Material Implementation

Material::Material(GLuint diffuse_, GLuint emissive_, float shininess_) {
    diffuse = diffuse_;
    emissive = emissive_;
    shininess = shininess_;
}

void Material::bind(Shader *shader, bool use_shininess) {
    if (use_shininess) {
        shader->setUniformFloat("materialShininess", shininess);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, emissive);

    shader->setUniformInt("colorTexture", 0);
    shader->setUniformInt("emissiveTexture", 1);
}

// MaterialFactory Implementation

Material *MaterialFactory::default_material = nullptr;
Material *MaterialFactory::default_material_2d = nullptr;

GLuint MaterialFactory::default_diffuse = 0;
GLuint MaterialFactory::default_emissive = 0;
float MaterialFactory::default_shininess = 32.f;

GLuint MaterialFactory::loadMaterialTexture(fs::path definition_path, fs::path textures_folder, bool clamp) {
    Asset texture_definition_asset(definition_path);
    const char *asset_buffer = texture_definition_asset.getBuffer();
    if (asset_buffer == nullptr || strlen(asset_buffer) == 0) {
        return -1;
    }

    fs::path texture_path = textures_folder;
    texture_path.append(std::string(asset_buffer));
    std::string texture_path_string = std::string(texture_path.c_str());
    return TextureLoader::loadTexture(texture_path_string, true, clamp);
}

float MaterialFactory::loadMaterialValue(fs::path definition_path) {
    Asset value_definition_asset(definition_path);
    const char *asset_buffer = value_definition_asset.getBuffer();
    if (asset_buffer == nullptr) {
        return -1.f;
    }

    return ((float *)asset_buffer)[0];
}

Material *MaterialFactory::createMaterial(fs::path input_directory) {
    fs::path default_tex_path = Asset::assetPath();
    default_tex_path.append(DEFAULT_TEXTURES_PATH);
    return createMaterial(input_directory, default_tex_path, false);
}

Material *MaterialFactory::createMaterial2D(fs::path input_directory) {
    fs::path default_tex_path = Asset::assetPath();
    default_tex_path.append(DEFAULT_TEXTURES_PATH);
    return createMaterial(input_directory, default_tex_path, true);
}

Material *MaterialFactory::createMaterial(fs::path input_directory, fs::path textures_folder, bool clamp_textures) {
    GLuint diffuse_texture, emissive_texture;
    float shininess = 0;

    // Handle diffuse information
    fs::path diffuse_definition_path = input_directory; //To Materials subfolder
    diffuse_definition_path.append(DIFFUSE_DEFINITION_FILENAME);
    diffuse_texture = loadMaterialTexture(diffuse_definition_path, textures_folder, clamp_textures);
    if (diffuse_texture == (GLuint)-1) {
        std::cout << "No diffuse loaded - using default" << std::endl;
        diffuse_texture = default_diffuse;
    }

    // Handle emissive information
    fs::path emissive_definition_path = input_directory;
    emissive_definition_path.append(EMISSIVE_DEFINITION_FILENAME);
    emissive_texture = loadMaterialTexture(emissive_definition_path, textures_folder, clamp_textures);
    if (emissive_texture == (GLuint)-1) {
        std::cout << "No emissive loaded - using default" << std::endl;
        emissive_texture = default_emissive;
    }

    // Handle shininess information
    fs::path shininess_path = input_directory;
    shininess_path.append(SHININESS_DEFINITION_FILENAME);
    shininess = loadMaterialValue(shininess_path);
    if (shininess == -1.f) {
        std::cout << "No shininess loaded - using default" << std::endl;
        shininess = default_shininess;
    }

    Material *material = new Material(diffuse_texture, emissive_texture, shininess);
    return material;
}

void MaterialFactory::loadDefaults() {
    // Get the paths for the default material
    fs::path default_path = Asset::assetPath().append(DEFAULT_MATERIAL_PATH);
    fs::path default_textures_path = Asset::assetPath().append(DEFAULT_TEXTURES_PATH);
    default_material = MaterialFactory::createMaterial(default_path, default_textures_path, false);
    default_material_2d = MaterialFactory::createMaterial(default_path, default_textures_path, true);

    default_diffuse = default_material_2d->getDiffuse();
    default_emissive = default_material_2d->getEmissive();
}
