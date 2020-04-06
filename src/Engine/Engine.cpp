#include "Engine.hpp"

Engine::Engine() {
    quit = false;
}

Engine::~Engine() {
    if( state ) delete state;
}

Engine & Engine::getEngine() {
    // Initialize the singleton instance if necessary
    static Engine engine;
    return engine;
}

// Perform any initialization necessary for the engine
// Returns true if all initialization was successful, else false
bool Engine::init( std::string level_to_load ) {
    // Attempt to initialize the window
    if( !window.init( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT ) ) {
        std::cout << "Error in window initialization" << std::endl;
        return false;
    }

    // Create the rendersystem with the window size
    glm::vec2 window_size = window.getDrawableSize();
    RenderSystem &rs = RenderSystem::getRenderSystem();
    rs.reshape( (int)window_size.x, (int)window_size.y );

    // Create a new state
    state = new InGameState( level_to_load );

    return true;
}

void Engine::handleSDLEvents() {
    std::vector<SDL_Event> events = window.getSDLEvents();
    SDL_Event e;

    for( int i=0; i<events.size(); i++ ) {
        // Get the last event in the vector
        e = events[i];

        if( e.type == SDL_QUIT ) {
            // Quit the engine
            quitEngine();
        }
        else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q ) {
            // Temporary quit button
            quitEngine();
        }
        else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE ) {
            // Temporary mouse lock toggle button
            window.toggleMouseLock();
        }
        else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
            // Resize everything
            glm::vec2 draw_size = window.getDrawableSize();
            state->reshape( (int)draw_size.x, (int)draw_size.y );
        }
        else {
            state->handleSDLEvent(e);
        }
    }
}

void Engine::quitEngine() {
    quit = true;
    window.close();
}

// Tick the engine
// Update all systems and states, then render
void Engine::tick() {
    // Test for input events
    handleSDLEvents();
    
    if( !quit ) {
        // Update the game state
        state->gameLoop();

        // Tell the window to handle any post rendering necessicities
        window.postRender();
    }
}
