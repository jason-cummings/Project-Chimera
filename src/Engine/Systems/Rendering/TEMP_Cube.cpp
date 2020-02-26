#include "TEMP_Cube.hpp"

const GLfloat TempCube::vbo_data[] = {
    // x     y     z     w       r     g     b     a      nx    ny    nz      tx    ty    tz     btx   bty   btz       u     v
    -1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f,  1.f,  1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
     1.f,  1.f,  1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
    -1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
     1.f,  1.f,  1.f,  1.f,    1.f,  0.f,  0.f,  1.f,    0.f,  1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,

    -1.f, -1.f, -1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f, -1.f,  1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
     1.f, -1.f,  1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
    -1.f, -1.f, -1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f, -1.f, -1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
     1.f, -1.f,  1.f,  1.f,    1.f,  1.f,  0.f,  1.f,    0.f, -1.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,

     1.f, -1.f, -1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f, -1.f,  1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
     1.f,  1.f,  1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
     1.f, -1.f, -1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f,  1.f, -1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
     1.f,  1.f,  1.f,  1.f,    0.f,  1.f,  0.f,  1.f,    1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,

    -1.f, -1.f, -1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f, -1.f,  1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
    -1.f,  1.f,  1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
    -1.f, -1.f, -1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f,  1.f, -1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
    -1.f,  1.f,  1.f,  1.f,    0.f,  1.f,  1.f,  1.f,   -1.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,

    -1.f, -1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f,  1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
     1.f,  1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
    -1.f, -1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f, -1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
     1.f,  1.f,  1.f,  1.f,    0.f,  0.f,  1.f,  1.f,    0.f,  0.f,  1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,

    -1.f, -1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
    -1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  1.f,
     1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
    -1.f, -1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    0.f,  0.f,
     1.f, -1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  0.f,
     1.f,  1.f, -1.f,  1.f,    1.f,  0.f,  1.f,  1.f,    0.f,  0.f, -1.f,    0.f,  0.f,  0.f,    0.f,  0.f,  0.f,    1.f,  1.f,
};

const GLint TempCube::ibo_data[] = {
     0,  1,  2,  3,  4,  5,
     6,  7,  8,  9, 10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};

const int TempCube::n_vertices = sizeof(vbo_data)/sizeof(GLfloat)/N_VERTEX_VALUES;

TempCube::TempCube() {
	// Create VBO
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW ); // 4 * n_vertices * sizeof(GLfloat)

	// Create IBO
	glGenBuffers( 1, &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW );
}

void TempCube::render() {
    glEnableVertexAttribArray( ShaderAttrib::Vertex );
    glEnableVertexAttribArray( ShaderAttrib::Color );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glVertexAttribPointer( ShaderAttrib::Vertex, 4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(0) );
    glVertexAttribPointer( ShaderAttrib::Color,  4, GL_FLOAT, GL_FALSE, N_VERTEX_VALUES * sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)) );

    glDrawElements( GL_TRIANGLES, n_vertices, GL_UNSIGNED_INT, 0 );

    glDisableVertexAttribArray( ShaderAttrib::Vertex );
    glDisableVertexAttribArray( ShaderAttrib::Color );

    
}
