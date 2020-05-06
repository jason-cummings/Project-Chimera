#include "Engine.hpp"

#include "SettingsManager.hpp"
#include "States/MainMenu.hpp"

Engine::Engine() {
    quit = false;
}

Engine::~Engine() {
    if( state ) {
        quitEngine();
    }
}

// Returns the singleton instance of the Engine
Engine & Engine::getEngine() {
    // Initialize the singleton instance if necessary
    static Engine engine;
    return engine;
}

// Perform any initialization necessary for the engine
// Returns true if all initialization was successful, else false
bool Engine::init() {
    // Get user settings for initialization
    UserSettings::loadFromFile();

    // Attempt to initialize the window
    if( !window.init( UserSettings::resolution_width, UserSettings::resolution_height ) ) {
        std::cout << "Error in window initialization" << std::endl;
        return false;
    }

    // Call to initialize RenderSystem singleton
    RenderSystem &rs = RenderSystem::getRenderSystem();

    // Load the default material now that all GL stuff is initialized
    Material::loadDefaultMaterial();

    // Create a new state
    state = new MainMenu();
    if( !state->getInitSuccess() ) {
        return false;
    }
    window.setMouseLock( state->shouldLockMouse() );

    // Reshape the state with the window size
    glm::vec2 window_size = window.getDrawableSize();
    state->reshape( (int)window_size.x, (int)window_size.y );

    return true;
}

// Quit the engine and clean up any memory necessary
void Engine::quitEngine() {
    quit = true;
    delete state;
    state = nullptr;
    window.close();
}

// Tests if the current state is passing a new state to switch to
// If so, swaps the state and tests whether it should destroy the current state
void Engine::testAndHandleStateChange() {
    GameState *next = state->getNextState();
    if( next != nullptr ) {
        // Test if the current state should be destroyed
        if( state->shouldDestroy() ) {
            delete state;
        }

        // Swap in the new state and perform necessary config
        state = next;
        glm::vec2 draw_size = window.getDrawableSize();
        state->reshape( (int)draw_size.x, (int)draw_size.y );
        window.setMouseLock( state->shouldLockMouse() );
    }
}

// Handles any SDL events retrieved from Window
void Engine::handleSDLEvents() {
    std::vector<SDL_Event> events = window.getSDLEvents();
    for( SDL_Event e : events ) {
        if( e.type == SDL_QUIT ) {
            // Quit the engine
            quit = true;
            return;
        }
        else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q ) {
            // Temporary quit button
            quit = true;
            return;
        }
        else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
            // Resize everything
            glm::vec2 draw_size = window.getDrawableSize();
            state->reshape( (int)draw_size.x, (int)draw_size.y );
        }
        else {
            // Delegate all other even handling to the state
            state->handleSDLEvent(e);
        }
    }
}

// Tick the engine
// Update all systems and states, then render
void Engine::tick() {
    // Test for input events
    handleSDLEvents();

    if( quit || state->getQuitGame() ){
        quitEngine();
    }
    else {
        // Determine if the state needs to be changed
        testAndHandleStateChange();

        // Update the game state
        state->gameLoop();

        // Tell the window to handle any post rendering necessicities
        window.postRender();
    }
}
