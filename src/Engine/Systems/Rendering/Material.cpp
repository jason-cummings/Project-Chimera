#include "Material.hpp"

Material::Material(GLuint texture_, GLuint emissive_, float shininess_){
    texture = texture_;
    emissive = emissive_;
    shininess = shininess_;
}

void Material::bind(Shader *shader) {
    shader->setUniformFloat( "materialShininess", shininess );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, emissive );

    shader->setUniformInt( "colorTexture", 0 );
    shader->setUniformInt( "emissiveTexture", 1 );
}
