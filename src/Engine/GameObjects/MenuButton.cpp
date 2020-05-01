#include "MenuButton.hpp"

// The VBO for a rectangular button
const GLfloat button_vbo_data[] = {
//   x   y    u   v
    -.5, -.5,   0, 0,
     .5, -.5,   1, 0,
    -.5,  .5,   0, 1,
	-.5,  .5,   0, 1,
     .5, -.5,   1, 0,
     .5,  .5,   1, 1
};
const GLint button_ibo_data[] = { 0, 1, 2, 3, 4, 5 };

GLuint MenuButton::button_vbo = 0;
GLuint MenuButton::button_ibo = 0;

MenuButton::MenuButton(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name ): GameObject(id) {
    xcoord = xinput;
    ycoord = yinput;
    width = winput;
    height = hinput;

    if( button_vbo == 0 ) {
        createBuffers();
    }

    mesh = new OverlayMesh( button_vbo, button_ibo, 6 );

    fs::path mat_path = Asset::assetPath();
    mat_path.append( "Materials/" + material_name );
    Material *button_material = MaterialFactory::createMaterial( mat_path );
    mesh->setMaterial( button_material );

    setTransform( glm::vec3(width, height, 1.f), glm::quat(), glm::vec3(xcoord, ycoord, 0.f) );
}

MenuButton::~MenuButton() {
    // delete mesh;
}

bool MenuButton::clickTest(float xinput, float yinput){
    return (yinput >= ycoord - height / 2.f && yinput <= ycoord + height / 2.f && xinput >= xcoord - width / 2.f && xinput <= xcoord + width / 2.f); 
}

void MenuButton::createBuffers() {
    glGenBuffers( 1, &button_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, button_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 24, &button_vbo_data, GL_STATIC_DRAW );

    glGenBuffers( 1, &button_ibo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, button_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, &button_ibo_data, GL_STATIC_DRAW );
}