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

#endif