#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include "../Systems/Rendering/TextManager.hpp"
#include "MenuElement.hpp"

enum class TextAlign {
    CENTER,
    LEFT,
    RIGHT
};

class TextElement : public MenuElement {
  private:
    GLuint current_texture;
    TextManager *text_manager;
    TextAlign alignment;

  public:
    TextElement(std::string text);

    void setText(std::string new_text, bool delete_current);
};

#endif
