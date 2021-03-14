#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "GameState.hpp"
#include "Systems/Rendering/RenderSystem.hpp"
#include "Window.hpp"

class Engine {
private:
    bool quit;
    Window window;

    bool start_in_level;

    // Engine state
    GameState *state;

    // Private constructor for singleton instance
    Engine();
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

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
    static Engine &getEngine();

    // Initialize the engine and return success
    bool init();

    // Engine flag management
    void handleStartInLevelFlag(const char *level_arg);

    // Initialize the state if necessary (wasn't set by a flag)
    bool initState();

    // Step the Engine based on time
    void tick();

    // Test if the engine should quit
    inline bool getQuit() { return quit; }
};

#endif