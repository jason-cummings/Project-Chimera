#include "Engine.hpp"

Engine *Engine::engine = nullptr;

Engine::Engine() {
    render_system = nullptr;
    physics_system = nullptr;
    quit = false;
    timer = new StandardTimer();
}

Engine::~Engine() {
    if( render_system ) delete render_system;
    if( physics_system ) delete physics_system;
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

// Perform any initialization necessary for the engine
// Returns true if all initialization was successful, else false
bool Engine::init() {
    // Attempt to initialize the window
    if( !window.init( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT ) ) {
        std::cout << "Error in window initialization" << std::endl;
        return false;
    }

    // Create the rendersystem with the window size
    glm::vec2 window_size = window.getDrawableSize();
    render_system = new RenderSystem( (int)window_size.x, (int)window_size.y );

    // Create the physics system
    physics_system = new PhysicsSystem();

    // Create a new state
    state = new InGameState();

    // Add all of states physics components to the physics system (THIS WILL LIKELY CHANGE)
    physics_system->addSceneComponents( state->getScene() );

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
        else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
            // Resize everything
            int w_width = e.window.data1;
            int w_height = e.window.data2;
            window.reshape( w_width, w_height );
            glm::vec2 draw_size = window.getDrawableSize();
            render_system->reshape( (int)draw_size.x, (int)draw_size.y );
        }
        else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q ) {
            // Temporary quit button
            quitEngine();
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

void Engine::tick() {
    double dt = timer->getLastTickTime();

    // Test for input events
    handleSDLEvents();
    
    if( !quit ) {
        // Update the game state
        state->update(dt);

        // Step physics
        physics_system->stepPhysics(dt);

        // Render all
        render_system->render( dt, state->getScene() );

        // Tell the window to handle any post rendering necessicities
        window.postRender();
    }
}
