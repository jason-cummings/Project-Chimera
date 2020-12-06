#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GL/glew.h>
#include <map>
#include <string>

class TextureLoader {
  private:
    static std::map<std::string, GLuint> textures; // uses map so it doesn't read a texture multiple time, just returns the GLuint
  public:
    static GLuint loadTexture(std::string &filename, bool undoGamma, bool clamp);
    static void loadTextureForCubeMap(std::string &filename, bool gammaCorrection, GLenum cube_map_side);
};

#endif