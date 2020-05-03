#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Systems/Rendering/RenderSystem.hpp"
#include "Window.hpp"
#include "InGameState.hpp"
#include "MainMenu.hpp"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

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

    // Handles inputs given by user.
    void handleSDLEvents();

public:
    // Destructor - needs to be updated
    ~Engine();

    // Create the singleton instance if necessary and return it
    static Engine & getEngine();

    // Initialize the engine and return success
    bool init( std::string level_to_load );

    // Step the Engine based on time
    void tick();

    // Test if the engine should quit
    bool getQuit() { return quit; }
};

#endif