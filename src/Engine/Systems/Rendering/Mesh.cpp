#include "Mesh.hpp"

Mesh::Mesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in) {
	vbo = vbo_in;
	ibo = ibo_in;
	num_vertices = num_vertices_in;
}

Mesh::~Mesh() {

}

void Mesh::setUpDraw() {
	glEnableVertexAttribArray( ShaderAttrib::Vertex );
    glEnableVertexAttribArray( ShaderAttrib::Color );
    glEnableVertexAttribArray( ShaderAttrib::Normal );
    glEnableVertexAttribArray( ShaderAttrib::Tangent );
    glEnableVertexAttribArray( ShaderAttrib::Bitangent );
    glEnableVertexAttribArray( ShaderAttrib::Texture );
}
void Mesh::cleanUpDraw(){
	glDisableVertexAttribArray( ShaderAttrib::Vertex );
    glDisableVertexAttribArray( ShaderAttrib::Color );  
    glDisableVertexAttribArray( ShaderAttrib::Normal );
    glDisableVertexAttribArray( ShaderAttrib::Tangent );
    glDisableVertexAttribArray( ShaderAttrib::Bitangent );
    glDisableVertexAttribArray( ShaderAttrib::Texture );  
}


void Mesh::slowDraw() {
	setUpDraw();

    quickDraw();

    cleanUpDraw();
}


void Mesh::quickDraw() {
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glVertexAttribPointer( ShaderAttrib::Vertex,    4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttrib::Color,     4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Normal,    3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Tangent,   3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(11*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Bitangent, 3, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(14*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Texture,   2, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(17*sizeof(GLfloat)) );

    glDrawElements( GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0 );


}


void Mesh::bindMaterial() {
	// TODO
}