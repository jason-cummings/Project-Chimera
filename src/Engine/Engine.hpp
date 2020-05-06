#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Window.hpp"
#include "Systems/Rendering/RenderSystem.hpp"
#include "GameState.hpp"

class Engine {
private:
    bool quit;
    Window window;

    // Engine state
    GameState *state;

    // Private constructor for singleton instance
    Engine();

    // Quit the program
    void quitEngine();

    // Swap the state if necessary
    void testAndHandleStateChange();

    // Handles inputs given by user.
    void handleSDLEvents();

public:
    // Destructor - needs to be updated
    ~Engine();

    // Create the singleton instance if necessary and return it
    static Engine & getEngine();

    // Initialize the engine and return success
    bool init();

    // Step the Engine based on time
    void tick();

    // Test if the engine should quit
    bool getQuit() { return quit; }
};

#endif