#include "MenuElement.hpp"

#include "../Systems/Rendering/Material.hpp"
#include "../Utilities/Asset.hpp"

// The VBO for a rectangular button
const GLfloat element_vbo_data[] = {
//    x    y    u  v
    -.5, -.5,   0, 0,
     .5, -.5,   1, 0,
    -.5,  .5,   0, 1,
	-.5,  .5,   0, 1,
     .5, -.5,   1, 0,
     .5,  .5,   1, 1
};
const GLint element_ibo_data[] = { 0, 1, 2, 3, 4, 5 };

GLuint MenuElement::element_vbo = 0;
GLuint MenuElement::element_ibo = 0;

MenuElement::MenuElement(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name ):
MenuElement(
    id,
    xinput,
    yinput,
    winput,
    hinput,
    MaterialFactory::createMaterial2D( Asset::assetPath().append("Materials/" + material_name) )
) {}

MenuElement::MenuElement(std::string id, float xinput, float yinput, float winput, float hinput, Material *mat ): GameObject(id) {
    xcoord = xinput;
    ycoord = yinput;
    width = winput;
    height = hinput;

    int z_level = 100;

    if( element_vbo == 0 ) {
        createBuffers();
    }

    mesh = new OverlayMesh( element_vbo, element_ibo, 6 );
    mesh->setMaterial( mat );

    setTransform( glm::vec3(width, height, 1.f), glm::quat(), glm::vec3(xcoord, ycoord, 0.f) );
}

MenuElement::~MenuElement() {}

void MenuElement::createBuffers() {
    glGenBuffers( 1, &element_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, element_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 24, &element_vbo_data, GL_STATIC_DRAW );

    glGenBuffers( 1, &element_ibo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, &element_ibo_data, GL_STATIC_DRAW );
}
