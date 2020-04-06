#include "Mesh.hpp"

Mesh::Mesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in) {
	
	vbo = vbo_in;
	ibo = ibo_in;
	num_vertices = num_vertices_in;

	glGenVertexArrays( 1, &vao);
	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

	glEnableVertexAttribArray( ShaderAttrib::Vertex );
    glEnableVertexAttribArray( ShaderAttrib::Color );
    glEnableVertexAttribArray( ShaderAttrib::Normal );
    glEnableVertexAttribArray( ShaderAttrib::Tangent );
    glEnableVertexAttribArray( ShaderAttrib::Bitangent );
    glEnableVertexAttribArray( ShaderAttrib::Texture );

    glVertexAttribPointer( ShaderAttrib::Vertex,    4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttrib::Color,     4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Normal,    3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Tangent,   3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(11*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Bitangent, 3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(14*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Texture,   2, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(17*sizeof(GLfloat)) );

    glBindVertexArray( 0 );
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1,&vao);
}

void Mesh::draw() {
	glBindVertexArray( vao );

	glDrawElements( GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0 );

	glBindVertexArray( 0 );
}


void Mesh::bindMaterial() {
	// TODO
}