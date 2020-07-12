#ifndef MENUELEMENT_H
#define MENUELEMENT_H

#include <string>
#include <GL/glew.h>

#include "../GameObject.hpp"
#include "../Systems/Rendering/OverlayMesh.hpp"

class MenuElement: public GameObject
{
protected:
    float xcoord, ycoord, width, height;
    OverlayMesh *mesh;

    static GLuint element_vbo;
    static GLuint element_ibo;

    static void createBuffers();

public:
    MenuElement(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    ~MenuElement();

    bool hasRenderable() const override { return true; }
    Renderable * getRenderable() const override { return (Renderable *)mesh; }
};


#endif