#include "Engine.hpp"

Engine *Engine::engine = nullptr;

Engine::Engine() {
    rs = nullptr;
    quit = false;
    timer = new StandardTimer();
}

Engine::~Engine() {
    if( rs ) delete rs;
    if( timer ) delete timer;
    if( state ) delete state;
}

Engine * Engine::getEngine() {
    // Initialize the singleton instance if necessary
    if( engine == nullptr ) {
        engine = new Engine();
    }
    return engine;
}

bool Engine::getQuit() {
    return quit;
}

// Perform any initialization necessary for the engine
// Returns true if all initialization was successful, else false
bool Engine::init() {
    bool success = true;

    // Attempt to initialize the window
    if( !window.init( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT ) ) {
        std::cout << "Error in window initialization" << std::endl;
        success = false;
    }

    // Create the rendersystem with the window size
    glm::vec2 window_size = window.getDrawableSize();
    rs = new RenderSystem( window_size.x, window_size.y );

    // Create a new state
    state = new InGameState();

    return success;
}

void Engine::handleSDLEvents() {
    std::vector<SDL_Event> events = window.getSDLEvents();
    SDL_Event e;

    while( events.size() != 0 ) {
        // Get the last event in the vector
        e = events.back();

        if( e.type == SDL_QUIT ) {
            // Quit the engine
            quitEngine();
        }
        else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
            // Resize everything
            int w_width = e.window.data1;
            int w_height = e.window.data2;
            window.reshape( w_width, w_height );
            glm::vec2 draw_size = window.getDrawableSize();
            rs->reshape( draw_size.x, draw_size.y );
        }
        else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q ) {
            // Temporary quit button
            quitEngine();
        }
        else {
            state->handleSDLEvent(e);
        }

        // Remove the event just handled
        events.pop_back();
    }
}

void Engine::quitEngine() {
    quit = true;
    window.close();
}

void Engine::tick() {
    double dt = timer->getLastTickTime();

    // Test for input events
    handleSDLEvents();
    
    if( !quit ) {
        // Update the game state
        state->update(dt);

        // Render all
        rs->render( dt, state->getScene() );

        // Tell the window to handle any post rendering necessicities
        window.postRender();
    }
}
