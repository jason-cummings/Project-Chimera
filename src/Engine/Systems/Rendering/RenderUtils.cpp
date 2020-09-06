#include "RenderUtils.hpp"
#include <vector>

#define SPHERE_SEGMENTS 12
#define PI 3.14159265f

// The VBO for rendering a quad over the whole viewport
const GLfloat quad_vbo_data[] = {
//   x     y     w       u     v
    -1.0f,-1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,-1.0f, 0.0f,   1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     1.0f,-1.0f, 0.0f,   1.0f, 0.0f,
     1.0f, 1.0f, 0.0f,   1.0f, 1.0f
};

GLuint RenderUtils::quad_vao = 0;
GLuint RenderUtils::quad_vbo = 0;

GLuint RenderUtils::sphere_vao = 0;
GLuint RenderUtils::sphere_vbo = 0;
int RenderUtils::n_sphere_verts = 0;

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

	// Initialize a sphere to use for rendering point lights
	std::vector<glm::vec4> sphere_points;
	float inc = 2*PI / float(SPHERE_SEGMENTS);
	for( float i=0; i<2*PI-inc/2.f; i+=inc ) {
		for( float j=0; j<PI-inc/2.f; j+=inc ) {
			float i2 = i+inc;
			float j2 = j+inc;

			sphere_points.push_back( glm::vec4( sin( j)*cos( i), cos( j), sin( j)*sin( i), 1.f ) );
			sphere_points.push_back( glm::vec4( sin( j)*cos(i2), cos( j), sin( j)*sin(i2), 1.f ) );
			sphere_points.push_back( glm::vec4( sin(j2)*cos(i2), cos(j2), sin(j2)*sin(i2), 1.f ) );
			sphere_points.push_back( glm::vec4( sin( j)*cos( i), cos( j), sin( j)*sin( i), 1.f ) );
			sphere_points.push_back( glm::vec4( sin(j2)*cos(i2), cos(j2), sin(j2)*sin(i2), 1.f ) );
			sphere_points.push_back( glm::vec4( sin(j2)*cos( i), cos(j2), sin(j2)*sin( i), 1.f ) );
		}
	}
	n_sphere_verts = sphere_points.size();

	// Set up sphere vbo/vao
	glGenVertexArrays( 1, &sphere_vao );
	glBindVertexArray( sphere_vao );
	glGenBuffers( 1, &sphere_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, sphere_vbo );
	glBufferData( GL_ARRAY_BUFFER, 4*n_sphere_verts*sizeof(float), &sphere_points[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray( ShaderAttrib::Vertex );
	glVertexAttribPointer( ShaderAttrib::Vertex, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(0) );

	glBindVertexArray( 0 );
}

void RenderUtils::testGLError( const char *loc ) {
	int err;
	while( (err = glGetError()) != GL_NO_ERROR )
		std::cerr << "OpenGL error at " << loc << ": " << err << std::endl;
}

void RenderUtils::drawQuad() {
	glBindVertexArray( quad_vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
	//testGLError("Quad");
}

void RenderUtils::drawSphere() {
	glBindVertexArray( sphere_vao );
	glDrawArrays( GL_TRIANGLES, 0, n_sphere_verts );
	glBindVertexArray( 0 );
	//testGLError("Sphere");
}