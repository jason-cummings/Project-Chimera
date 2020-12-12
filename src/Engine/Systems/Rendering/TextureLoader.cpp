#include "TextureLoader.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, GLuint> TextureLoader::textures;

GLuint TextureLoader::loadTexture(std::string &filename, bool gammaCorrection, bool clamp) {
    if (textures.count(filename) > 0)
        return textures.at(filename);

    stbi_set_flip_vertically_on_load(true);

    int width;
    int height;
    int numChannels;
    unsigned char *image = stbi_load(filename.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    // std::cout << "Image: " << filename << ", channels: " << numChannels <<std::endl;
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    if (clamp) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (image) {
        GLenum internal_format;
        if (gammaCorrection) {
            internal_format = numChannels == 3 ? GL_SRGB : GL_SRGB_ALPHA;
        } else {
            internal_format = numChannels == 3 ? GL_RGB : GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
        textures.insert(std::pair<std::string, GLuint>(filename, tex));
        return tex;
    }
    std::cerr << "Failed to load texture: " << filename << std::endl;
    return 0;
}

void TextureLoader::loadTextureForCubeMap(std::string &filename, bool gammaCorrection, GLenum cube_map_side) {
    stbi_set_flip_vertically_on_load(false);

    int width;
    int height;
    int numChannels;
    unsigned char *image = stbi_load(filename.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);

    if (image) {
        GLenum internal_format;
        if (gammaCorrection) {
            internal_format = numChannels == 3 ? GL_SRGB : GL_SRGB_ALPHA;
        } else {
            internal_format = numChannels == 3 ? GL_RGB : GL_RGBA;
        }
        glTexImage2D(cube_map_side, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
}