#include "Framebuffer.hpp"

Framebuffer::Framebuffer() {
    glGenFramebuffers( 1, &framebufferID );
}

Framebuffer::~Framebuffer() {

}

void Framebuffer::bind() {
    glBindFramebuffer( GL_FRAMEBUFFER, framebufferID );
}

FBTexture * Framebuffer::addColorTexture( std::string texture_name, int width, int height ) {
    // Bind this framebuffer 
    bind();

    // Create the new texture and add it to the map
    GLuint new_texture_id;
    glGenTextures( 1, &new_texture_id );
    FBTexture *to_add = new FBTexture( new_texture_id, texture_name );
    // textures[texture_name] = to_add;
    
    // Set the appropriate texture paramenters
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Set the texture as a color attachment for the framebuffer and keep track of which it is
    GLenum current_attachment = GL_COLOR_ATTACHMENT0 + colorAttachments.size();
	glFramebufferTexture2D( GL_FRAMEBUFFER, current_attachment, GL_TEXTURE_2D, new_texture_id, 0 );
	colorAttachments[texture_name] = current_attachment;

    // Tell the framebuffer how many color attachments it has
	glDrawBuffers( colorAttachments.size(), &colorAttachments[0] );

    // Return the new texture 
    return to_add;
}
