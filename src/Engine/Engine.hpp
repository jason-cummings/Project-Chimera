#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Systems/Rendering/RenderSystem.hpp"
#include "Window.hpp"
#include "StandardTimer.hpp"
#include "InGameState.hpp"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

class Engine {
private:
    // Singleton Engine instance
    static Engine *engine;

    bool quit;
    Window window;

    // Engine systems
    RenderSystem *rs;
    Timer *timer;

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
    static Engine * getEngine();

    // Test if the engine should quit
    bool getQuit();

    // Initialize the engine and return success
    bool init();

    // Step the Engine based on time
    void tick();
};

#endif