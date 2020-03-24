#include "MeshFactory.hpp"

Mesh* MeshFactory::createBasicMesh( std::string input_directory ) {
	Asset newVBOAsset( input_directory + "/VBO" );
	Asset newIBOAsset( input_directory + "/IBO" );

	// Create VBO
	GLuint vbo;
	glGenBuffers( 1, &vbo  );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, newVBOAsset.getBytes(), newVBOAsset.getBuffer(), GL_STATIC_DRAW );

	// Create IBO
	GLuint ibo;
	glGenBuffers( 1, &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, newIBOAsset.getBytes(), newIBOAsset.getBuffer(), GL_STATIC_DRAW );

	Mesh* mesh = new Mesh(vbo,ibo,newIBOAsset.getBytes()/sizeof(int));
	return mesh;
}