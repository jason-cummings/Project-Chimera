#include "TextManager.hpp"

#include <iostream>

#include "../../Utilities/Asset.hpp"
#include "ShaderManager.hpp"

// The general VBO for rendering a quad for a character
const GLfloat text_vbo_data[] = {
//   x     y       u     v
    0.0f, 0.0f,   0.0f, 1.0f,
    1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 1.0f,   0.0f, 0.0f,
	0.0f, 1.0f,   0.0f, 0.0f,
    1.0f, 0.0f,   1.0f, 1.0f,
    1.0f, 1.0f,   1.0f, 0.0f
};

TextManager::TextManager( std::string font_path, unsigned int size ) {
    // Init FreeType
    FT_Library ft;
    if( FT_Init_FreeType(&ft) )
        std::cerr << "FreeType Error: Could not init FreeType Library" << std::endl;

    // Load the font
    std::string full_font_path = Asset::assetPath().append( font_path ).string();
    FT_Face face;
    if( FT_New_Face(ft, full_font_path.c_str(), 0, &face) ) {
        std::cerr << "FreeType Error: Failed to load font " << full_font_path << std::endl;
    }

    // Set the font size
    FT_Set_Pixel_Sizes(face, 0, size);

    // Disable byte-alignment restriction - Initial value: 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    int max_bearing = 0;
    // Load all 128 ASCII chars
    for( unsigned char c=0; c<128; c++ ) {
        // Load character glyph 
        if( FT_Load_Char(face, c, FT_LOAD_RENDER) ) {
            std::cerr << "FreeType Error: Failed to load Glyph " << c << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );

        // Texture options
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
        glBindTexture( GL_TEXTURE_2D, 0 );

        // Create character and insert to map
        Character to_insert = {
            texture,
            glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows ),
            glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top ),
            (int)face->glyph->advance.x >> 6
        };

        // Keep track of maximum y bearing for best approximation of font height
        if( to_insert.bearing.y > max_bearing ) {
            max_bearing = to_insert.bearing.y;
        }

        // Insert the character to the map
        char_map.insert(std::pair<char, Character>(c, to_insert));
    }
    
    // Get some metrics for font rendering - add 1 on top and bottom for consistent falloff
    baseline_y = -(face->size->metrics.descender / 64) + 1;
    true_height = max_bearing + baseline_y + 2;
    // std::cout << "Baseline Y  - " << baseline_y << std::endl;
    // std::cout << "True height - " << true_height << std::endl;

    // Set back to default
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Free FreeType resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    RenderUtils::testGLError( "FontLoader" );

    // Create the buffers for rendering text quads
    glGenFramebuffers( 1, &generator_fb );
    glGenVertexArrays( 1, &text_vao );
    glBindVertexArray( text_vao );
    glGenBuffers( 1, &text_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, text_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float) * 24, &text_vbo_data, GL_STATIC_DRAW );
	glEnableVertexAttribArray( ShaderAttrib2D::Vertex2D );
	glEnableVertexAttribArray( ShaderAttrib2D::Texture2D );
	glVertexAttribPointer( ShaderAttrib2D::Vertex2D,  2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(0) );
	glVertexAttribPointer( ShaderAttrib2D::Texture2D, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
	glBindVertexArray( 0 );

    RenderUtils::testGLError( "TextManager generator buffers" );
}

TextManager::~TextManager() {
    glDeleteFramebuffers( 1, &generator_fb );
    glDeleteVertexArrays( 1, &text_vao );
    glDeleteBuffers( 1, &text_vbo );

    std::map<char, Character>::iterator it;
    for( it = char_map.begin(); it != char_map.end(); it++ ) {
        glDeleteTextures( 1, &((*it).second.tex_id) );
    }
}

void TextManager::createTextTexture( std::string text, glm::vec4 color, GLuint &tex_out, unsigned int &width_out, unsigned int &height_out ) {
    if( text.length() == 0 ) return;

    // Get the total necessary width for the texture to return
    int tex_width = 0;
    std::string::iterator it;
    for( it = text.begin(); it != text.end(); it++ ) {
        Character c = char_map[*it];
        tex_width += c.advance;
    }

    // Create a text texture to return
    glBindFramebuffer( GL_FRAMEBUFFER, generator_fb );
    glGenTextures( 1, &tex_out );
    glBindTexture( GL_TEXTURE_2D, tex_out );

    // Texture options
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, tex_width, true_height, 0, GL_RGBA, GL_FLOAT, nullptr );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // Make it the render target
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_out, 0 );

    // Get the necessary shader
    Shader *shader = ShaderManager::getShaderManager()->getShader("text");
    shader->bind();
    glm::mat4 text_projection = glm::ortho( 0.f, (float)tex_width, 0.f, (float)true_height, -1.f, 1.f );
    shader->setUniformMat4( "Projection", text_projection );
    shader->setUniformVec4( "textColor", color );
    shader->setUniformInt( "textTexture", 0 );

    glViewport(0, 0, tex_width, true_height);
    glClear( GL_COLOR_BUFFER_BIT );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Iterate through the string rendering all characters
    float current_x = 0.f;
    // float current_x = (float)(char_map[text[0]].size.x - char_map[text[0]].advance);
    glBindVertexArray( text_vao );
    glActiveTexture( GL_TEXTURE0 );
    for( it = text.begin(); it != text.end(); it++ ) {
        Character c = char_map[*it];
        
        // Create the model matrix
        glm::vec3 char_scale = glm::vec3( c.size.x, c.size.y, 1.f );
        glm::vec3 char_translate = glm::vec3( current_x + (float)c.bearing.x, baseline_y - ((float)c.size.y - (float)c.bearing.y), 0.f );
        glm::mat4 model = glm::scale( glm::translate( glm::mat4(1.f), char_translate ), char_scale );
        shader->setUniformMat4( "Model", model );
        
        glBindTexture( GL_TEXTURE_2D, c.tex_id );
    	glDrawArrays( GL_TRIANGLES, 0, 6 );

        current_x += (float)(c.advance);
    }
 
    // Unbind all and return
    glDisable( GL_BLEND );
    glBindVertexArray(0);
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glUseProgram(0);

    width_out = tex_width;
    height_out = true_height;
}

