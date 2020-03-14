#include "Obstacle.hpp"

const GLfloat TEMP_vbo_data[] = {
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

const GLint TEMP_ibo_data[] = {
     0,  1,  2,  3,  4,  5,
     6,  7,  8,  9, 10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};

Obstacle::Obstacle(): GameObject(123) {
    GLuint vbo, ibo;
    glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(TEMP_vbo_data), TEMP_vbo_data, GL_STATIC_DRAW );

	// Create IBO
	glGenBuffers( 1, &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(TEMP_ibo_data), TEMP_ibo_data, GL_STATIC_DRAW );

    int n_vertices = sizeof(TEMP_vbo_data) / sizeof(GLfloat) / 19;

    mesh = new Mesh( vbo, ibo, n_vertices );
}

Obstacle::~Obstacle() {
    delete mesh;
}
