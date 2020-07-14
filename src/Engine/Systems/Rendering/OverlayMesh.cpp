#include "OverlayMesh.hpp"

OverlayMesh::OverlayMesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in) {
	num_vertices = num_vertices_in;
	material = Material::getDefaultMaterial2D();

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, vbo_in );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_in );

	glEnableVertexAttribArray( ShaderAttribOverlay::VertexOverlay );
    glEnableVertexAttribArray( ShaderAttribOverlay::TextureOverlay );

    glVertexAttribPointer( ShaderAttribOverlay::VertexOverlay,    2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttribOverlay::TextureOverlay,   2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );

    glBindVertexArray( 0 );
}

OverlayMesh::OverlayMesh(GLuint vao_in, int num_vertices_in) {
	vao = vao_in;
	num_vertices = num_vertices_in;
	material = Material::getDefaultMaterial2D();
}


OverlayMesh::~OverlayMesh() {
	glDeleteVertexArrays( 1, &vao );
}

void OverlayMesh::draw() {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

void OverlayMesh::setMaterial( Material *input ){
	material = input;
}
