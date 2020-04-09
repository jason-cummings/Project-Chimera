#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <map>
#include <GL/glew.h>
#include <iostream>


class TextureLoader{
private:
	static std::map<std::string, GLuint> textures; // uses map so it doesn't read a texture multiple time, just returns the GLuint
public:
	static GLuint loadTexture(std::string & filename, bool undoGamma);
};

#endif