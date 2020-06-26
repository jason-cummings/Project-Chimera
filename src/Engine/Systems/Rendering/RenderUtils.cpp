#include "RenderUtils.hpp"

GLuint RenderUtils::quad_vao = 0;
GLuint RenderUtils::quad_vbo = 0;

int RenderUtils::texture_width = 100;
int RenderUtils::texture_height = 100;

int RenderUtils::view_width = 100;
int RenderUtils::view_height = 100;

void RenderUtils::init() {
	//set up quad vao
	glGenVertexArrays( 1, &quad_vao );
	glBindVertexArray( quad_vao );
	
	// Create the VBO for the quad
	glGenBuffers( 1, &quad_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, quad_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 30, &quad_vbo_data, GL_STATIC_DRAW );

	glEnableVertexAttribArray( ShaderAttrib2D::Vertex2D );
	glEnableVertexAttribArray( ShaderAttrib2D::Texture2D );
	glVertexAttribPointer( ShaderAttrib2D::Vertex2D,  3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0) );
	glVertexAttribPointer( ShaderAttrib2D::Texture2D, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );

	glBindVertexArray( 0 );
}

void RenderUtils::testGLError( const char *loc ) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cerr << "OpenGL error at " << loc << ": " << err << std::endl;
}

void RenderUtils::drawQuad() {
	glBindVertexArray( quad_vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
	//testGLError("Quad");
}