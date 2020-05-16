#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <string>
#include <GL/glew.h>

#include "./../GameObject.hpp"
#include "./../Systems/Rendering/OverlayMesh.hpp"

class MenuButton : public GameObject
{
private:
    float xcoord, ycoord, width, height;
    OverlayMesh *mesh;

    static GLuint button_vbo;
    static GLuint button_ibo;

    static void createBuffers();

public:
    MenuButton(std::string id, float xinput, float yinput, float winput, float hinput, std::string material_name);
    ~MenuButton();

    inline bool hasRenderable() const override { return true; }
    inline Renderable * getRenderable() const override { return (Renderable *) mesh; }

    bool clickTest(float xinput, float yinput);
};


#endif