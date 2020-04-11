#ifndef MATERIALS_H
#define MATERIALS_H


#include <GL/glew.h>
#include "Shader.hpp"


class Material
{
private:
    
    GLuint texture = 0;
    GLuint emissive = 0;
    float shininess = 0;

public:
    Material(GLuint texture_, GLuint emissive_, float shininess_);
    void bind(Shader *shader);
    inline GLuint getTexture() { return texture; }
    inline GLuint getEmissive() { return emissive; }
    inline float getShininess() { return shininess; }
};





#endif