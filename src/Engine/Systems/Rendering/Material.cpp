#include "Material.hpp"

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
