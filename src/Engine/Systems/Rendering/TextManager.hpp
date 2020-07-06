#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <map>
#include <glm/vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "RenderUtils.hpp"
#include "Shader.hpp"
#include "../../GameObjects/MenuElement.hpp"

#define FONT_ARIAL_PATH "Fonts/arial.ttf"
#define FONT_THERAMIN_GOTHIC_PATH "Fonts/SFTheraminGothic.ttf"
#define FONT_THERAMIN_GOTHIC_SHADED_OBLIQUE_PATH "Fonts/SFTheraminGothicShadedOblique.ttf"
#define FONT_THERAMIN_GOTHIC_CONDENSED_PATH "Fonts/SFTheraminGothicCondensed.ttf"

struct Character {
    GLuint       tex_id;    // ID handle of the glyph texture
    glm::ivec2   size;      // Size of glyph
    glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
};

class TextManager {
private:
    // Map of ASCII chars to corresponding textures and values
    std::map<char, Character> char_map;

    // The baseline for text rendering
    int baseline_y, true_height;

    // Framebuffer for generating textures
    GLuint generator_fb;
    
	// VAO and VBO for text quads
    GLuint text_vao, text_vbo;

public:
    TextManager( std::string font_name, unsigned int size );
    ~TextManager();

    void createTextTexture( std::string text, glm::vec4 color, GLuint &tex_out, unsigned int &width_out, unsigned int &height_out );
};

#endif