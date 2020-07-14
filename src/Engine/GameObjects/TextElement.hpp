#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include "MenuElement.hpp"
#include "../Systems/Rendering/TextManager.hpp"

enum class TextAlign {
    CENTER,
    LEFT,
    RIGHT
};

class TextElement: public MenuElement {
private:
    GLuint current_texture;
    TextManager *text_manager;
    TextAlign alignment;

public:
    TextElement( std::string text );

    void setText( std::string new_text, bool delete_current );
};

#endif
