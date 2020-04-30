#include "MeshFactory.hpp"

Mesh* MeshFactory::createBasicMesh( std::string input_directory ) {
	Asset newVBOAsset( input_directory + "/VBO" );
	Asset newIBOAsset( input_directory + "/IBO" );

	// Create VBO
	GLuint vbo = createVBO(newVBOAsset);

	// Create IBO
	GLuint ibo = createIBO(newIBOAsset);

	Mesh* mesh = new Mesh(vbo,ibo,newIBOAsset.getBytes()/sizeof(int));
	return mesh;
}

Mesh* MeshFactory::createBasicMesh( fs::path input_directory ) {
	fs::path vbo_path = input_directory;
	vbo_path.append("VBO");
	fs::path ibo_path = input_directory;
	ibo_path.append("IBO");
	Asset newVBOAsset( vbo_path );
	Asset newIBOAsset( ibo_path );

	// Create VBO
	GLuint vbo = createVBO(newVBOAsset);

	// Create IBO
	GLuint ibo = createIBO(newIBOAsset);

	Mesh* mesh = new Mesh(vbo,ibo,newIBOAsset.getBytes()/sizeof(int));
	return mesh;
}

SkinnedMesh* MeshFactory::createSkinnedMesh( std::string input_directory, JointList * joint_list ) {
	Asset newVBOAsset( input_directory + "/VBO" );
	Asset newIBOAsset( input_directory + "/IBO" );

	// Create VBO
	GLuint vbo = createVBO(newVBOAsset);

	// Create IBO
	GLuint ibo = createIBO(newIBOAsset);

	SkinnedMesh* skinned_mesh = new SkinnedMesh(vbo,ibo,newIBOAsset.getBytes()/sizeof(int),joint_list);
	return skinned_mesh;
}


SkinnedMesh* MeshFactory::createSkinnedMesh( fs::path input_directory, JointList * joint_list ) {
	fs::path vbo_path = input_directory;
	vbo_path.append("VBO");
	fs::path ibo_path = input_directory;
	ibo_path.append("IBO");
	Asset newVBOAsset( vbo_path );
	Asset newIBOAsset( ibo_path );

	// Create VBO
	GLuint vbo = createVBO(newVBOAsset);

	// Create IBO
	GLuint ibo = createIBO(newIBOAsset);

	SkinnedMesh* skinned_mesh = new SkinnedMesh(vbo,ibo,newIBOAsset.getBytes()/sizeof(int),joint_list);
	return skinned_mesh;
}

GLuint MeshFactory::createVBO(Asset& asset) {
	GLuint vbo;
	glGenBuffers( 1, &vbo  );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, asset.getBytes(), asset.getBuffer(), GL_STATIC_DRAW );
	return vbo;
}

GLuint MeshFactory::createIBO(Asset& asset) {
	GLuint ibo;
	glGenBuffers( 1, &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, asset.getBytes(), asset.getBuffer(), GL_STATIC_DRAW );
	return ibo;
}