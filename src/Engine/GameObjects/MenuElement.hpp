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
    int z_level;
    OverlayMesh *mesh;

    static GLuint element_vao;
    // static GLuint element_vbo;
    // static GLuint element_ibo;

    static void createBuffers();

public:
    MenuElement(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    MenuElement(std::string id, float xinput, float yinput, float winput, float hinput, Material *mat);
    ~MenuElement();

    bool hasRenderable() const override { return true; }
    Renderable * getRenderable() const override { return (Renderable *)mesh; }

    inline void setZLevel( int z_in ) { mesh->setZLevel(z_in); }
};


#endif