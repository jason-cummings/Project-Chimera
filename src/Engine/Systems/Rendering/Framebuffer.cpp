#include "Framebuffer.hpp"

Framebuffer::Framebuffer() {
    glGenFramebuffers( 1, &framebuffer_id );
    depth_buffer = 0;
}

Framebuffer::~Framebuffer() {

}

void Framebuffer::bind() {
    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer_id );
    GLuint err;
    if( (err = glGetError()) != GL_NO_ERROR ) {
        std::cout << "GL error " << err << " binding " << std::endl;
    }
}

void Framebuffer::addColorTexture( std::string texture_name, int width, int height ) {
    // Bind this framebuffer 
    bind();

    // Create the new texture and add it to the map
    GLuint new_texture_id;
    glGenTextures( 1, &new_texture_id );
    FBTexture *to_add = new FBTexture( new_texture_id, texture_name );
    textures[texture_name] = to_add;
    
    // Set the appropriate texture paramenters
    glBindTexture( GL_TEXTURE_2D, new_texture_id );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Set the texture as a color attachment for the framebuffer and keep track of which it is
    GLenum current_attachment = GL_COLOR_ATTACHMENT0 + color_attachments.size();
	glFramebufferTexture2D( GL_FRAMEBUFFER, current_attachment, GL_TEXTURE_2D, new_texture_id, 0 );
	color_attachments.push_back( current_attachment );

    // Tell the framebuffer how many color attachments it has
	glDrawBuffers( color_attachments.size(), &color_attachments[0] );

    // Unbind the framebuffer and created texture
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void Framebuffer::addDepthBuffer( int width, int height ) {
    if( depth_buffer == 0 ) {
		bind();
		glGenRenderbuffers( 1, &depth_buffer );
		glBindRenderbuffer( GL_RENDERBUFFER, depth_buffer );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer );
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
}

FBTexture * Framebuffer::getTexture( std::string texture_name ) {
    return textures[texture_name];
}