#include "MeshFactory.hpp"

Mesh* MeshFactory::createBasicMesh( std::string input_directory ) {
	std::string vboInput = input_directory + "/VBO";
	Asset* newVBOAsset = new Asset(vboInput);

	std::string iboInput = input_directory + "/IBO";
	Asset* newIBOAsset = new Asset(iboInput);

	// Create VBO
	GLuint vbo;

	glGenBuffers( 1, &vbo  );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, newVBOAsset->getSize(), newVBOAsset->getBuffer(), GL_STATIC_DRAW );

	// Create IBO
	GLuint ibo;

	glGenBuffers( 1, &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, newIBOAsset->getSize(), newIBOAsset->getBuffer(), GL_STATIC_DRAW );
	Mesh* mesh = new Mesh(vbo,ibo,newIBOAsset->getSize()/sizeof(int));

	delete newVBOAsset;
	delete newIBOAsset;

	return mesh;
}