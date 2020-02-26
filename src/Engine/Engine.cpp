#include "Engine.hpp"

Engine *Engine::engine = nullptr;

Engine::Engine() {
    window = nullptr;
    rs = nullptr;
}

Engine::~Engine() {
    delete rs;
}

Engine * Engine::getEngine() {
    // Initialize the singleton instance if necessary
    if( engine == nullptr ) {
        engine = new Engine();
    }

    return engine;
}

bool Engine::getQuit() {
    return window->getQuit();
}

// Perform any initialization necessary for the engine
// Returns true if all initialization was successful, else false
bool Engine::init() {
    bool success = true;

    // Attempt to initialize the window
    window = new Window( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
    if( !window->init() ) {
        std::cout << "Error in window initialization" << std::endl;
        success = false;
    }

    rs = new RenderSystem( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
    window->setRS(rs);

    return success;
}

void Engine::tick( double dt ) {
    // Test for input events
    window->handleEvents();

    // Render all
    rs->render(dt);

    // Tell the window to handle any post rendering necessicities
    window->postRender();
}
