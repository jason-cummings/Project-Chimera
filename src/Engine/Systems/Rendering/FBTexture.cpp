#include "FBTexture.hpp"

FBTexture::FBTexture(GLuint id_, std::string name_) {
    id = id_;
    name = name_;
}

// Bind this texture to the specified texture unit
void FBTexture::bind(GLenum texture_unit) {
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, id);
}