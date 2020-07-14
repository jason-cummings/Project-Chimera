#include "Menu.hpp"

Menu::Menu() {
	scene = new GameObject("MenuRoot");
    mouse_lock = false;
	lastpressed = nullptr;
}

Menu::~Menu() {}

glm::vec2 Menu::getMenuCoords( int screen_x, int screen_y ) {
    int w_width = RenderUtils::getViewWidth();
    int w_height = RenderUtils::getViewHeight();
    int invert_y = w_height - screen_y;

    float menu_x = screen_x / (float)w_height - (((w_width / (float)w_height) - 1) / 2.f);
    float menu_y = invert_y / (float)w_height;

    return glm::vec2( menu_x, menu_y );
}

void Menu::handleMouseButtonDownStateSpecific( SDL_Event e ) {
    if( e.button.button == SDL_BUTTON_LEFT ) {
        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        glm::vec2 menu_coords = getMenuCoords( x, y );

        // Find button clicked - set to lastpressed
        lastpressed = nullptr;
        for( int i = 0; i < buttons.size(); i++ ) {
            if( buttons[i]->clickTest( menu_coords.x, menu_coords.y ) ) {
                lastpressed = buttons[i];
            }
        }
    }
}

void Menu::handleMouseButtonUpStateSpecific( SDL_Event e ) {
    if( e.button.button == SDL_BUTTON_LEFT ) {
        // Click coordinates (in pixels)
        int x = 0, y = 0;
        SDL_GetMouseState( &x, &y );

        // Coordinates in our menu coord system
        glm::vec2 menu_coords = getMenuCoords( x, y );

        // Test if the button pressed down is the one being released over
        if( lastpressed != nullptr && lastpressed->clickTest( menu_coords.x, menu_coords.y ) ) {
            handleButtonEvent(lastpressed);
        }
    }
}

void Menu::gameLoop() {
    render_system.render( 0.f );
}
