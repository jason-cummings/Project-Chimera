#include "SkinnedMesh.hpp"

SkinnedMesh::SkinnedMesh(GLuint vbo_in, GLuint ibo_in, int num_vertices_in, JointList * joints_in) {
	
	vbo = vbo_in;
	ibo = ibo_in;
	num_vertices = num_vertices_in;
	joints = joints_in;

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

    //joints
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Index_1 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Weight_1 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Index_2 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Weight_2 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Index_3 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Weight_3 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Index_4 );
    // glEnableVertexAttribArray( ShaderAttrib::Joint_Weight_4 );

    glEnableVertexAttribArray( ShaderAttrib::Joint1to4 );
    glEnableVertexAttribArray( ShaderAttrib::Joint5to8 );
    glEnableVertexAttribArray( ShaderAttrib::Joint9to12 );
    glEnableVertexAttribArray( ShaderAttrib::Joint13to16 );
    glEnableVertexAttribArray( ShaderAttrib::Joint17to20 );
    glEnableVertexAttribArray( ShaderAttrib::Joint21to22 );

    // standard vertex attribute pointers
    glVertexAttribPointer( ShaderAttrib::Vertex,    4, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttrib::Color,     4, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(4*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Normal,    3, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(8*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Tangent,   3, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(11*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Bitangent, 3, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(14*sizeof(GLfloat)) );
    glVertexAttribPointer( ShaderAttrib::Texture,   2, GL_FLOAT, GL_FALSE, N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(17*sizeof(GLfloat)) );

    // joint attribute Pointers
    // glVertexAttribPointer( ShaderAttrib::Joint_Index_1,   1, GL_INT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(19*sizeof(GLfloat)) );
    // glVertexAttribPointer( ShaderAttrib::Joint_Weight_1,   1, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(19*sizeof(GLfloat) + 1*sizeof(GLint)) );

    // glVertexAttribPointer( ShaderAttrib::Joint_Index_2,   1, GL_INT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(20*sizeof(GLfloat) + 1*sizeof(GLint)) );
    // glVertexAttribPointer( ShaderAttrib::Joint_Weight_2,   1, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(20*sizeof(GLfloat) + 2*sizeof(GLint)) );

    // glVertexAttribPointer( ShaderAttrib::Joint_Index_3,   1, GL_INT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(21*sizeof(GLfloat) + 2*sizeof(GLint)) );
    // glVertexAttribPointer( ShaderAttrib::Joint_Weight_3,   1, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(21*sizeof(GLfloat) + 3*sizeof(GLint)) );

    // glVertexAttribPointer( ShaderAttrib::Joint_Index_4,   1, GL_INT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(22*sizeof(GLfloat) + 3*sizeof(GLint)) );
    // glVertexAttribPointer( ShaderAttrib::Joint_Weight_4,   1, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(22*sizeof(GLfloat) + 4*sizeof(GLint)) );

    glVertexAttribPointer( ShaderAttrib::Joint1to4,   4, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(19*sizeof(GLfloat)) );

    glVertexAttribPointer( ShaderAttrib::Joint5to8,   4, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(23*sizeof(GLfloat)) );

    glVertexAttribPointer( ShaderAttrib::Joint9to12,   4, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(27*sizeof(GLfloat)) );

    glVertexAttribPointer( ShaderAttrib::Joint13to16,   4, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(31*sizeof(GLfloat)) );

    glVertexAttribPointer( ShaderAttrib::Joint17to20,   4, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(35*sizeof(GLfloat)) );

    glVertexAttribPointer( ShaderAttrib::Joint21to22,   2, GL_FLOAT, GL_FALSE,  N_FLOAT_ATTRIBUTES * sizeof(GLfloat) + N_INT_ATTRIBUTES * sizeof(GLint), (GLvoid*)(39*sizeof(GLfloat)) );

    glBindVertexArray( 0 );

    material = Material::getDefaultMaterial();
}

SkinnedMesh::~SkinnedMesh() {
	glDeleteVertexArrays(1,&vao);
}

void SkinnedMesh::draw() {
	glBindVertexArray( vao );

	glDrawElements( GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0 );

	glBindVertexArray( 0 );
}

void SkinnedMesh::setMaterial( Material* input ){
	material = input;
}