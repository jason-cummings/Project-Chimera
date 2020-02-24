#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Systems/Rendering/RenderSystem.hpp"
#include "Window.hpp"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

class Engine {
private:
    // Singleton Engine instance
    static Engine *engine;

    Window *window;

    // Engine systems
    RenderSystem *rs;

    // Private constructor for singleton instance
    Engine();


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
    void tick( double dt );
};

#endif