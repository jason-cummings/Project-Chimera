#include "Engine/Engine.hpp"
#include <iostream>

#define SDL_MAIN

void handleFlags(int argc, char *argv[], Engine &engine);
void printHelp();

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printHelp();
        return 0;
    }

    std::cout << "Welcome to Project Chimera" << std::endl;

    Engine &engine = Engine::getEngine();

    if (!engine.init()) {
        std::cerr << "Could not initialize Engine" << std::endl;
        return 1;
    }

    handleFlags(argc, argv, engine);

    if (!engine.initState()) {
        std::cerr << "Could not initialize State" << std::endl;
        return 1;
    }

    while (!engine.getQuit()) {
        engine.tick();
    }

    return 0;
}

int handleFlag(const char *arg1, const char *arg2, Engine &engine) {
    if (arg1[0] != '-') {
        // First arg is not a flag. Continue
        std::cerr << "Unused value " << arg1 << std::endl;
        return 1;
    }

    if (strcmp(arg1, "-l") == 0) {
        // Start in level flag
        if (strcmp(arg2, "") == 0 || arg2[0] == '-') {
            std::cerr << "-l requires an argument" << std::endl;
            std::cerr << "Valid arguments are: Towers, Castle, FbxTest" << std::endl;
            return 1;
        } else {
            engine.handleStartInLevelFlag(arg2);
            return 2;
        }
    }

    // Flag has not been handled
    std::cerr << "Unrecognized flag " << arg1 << std::endl;
    return 1;
}

void handleFlags(int argc, char *argv[], Engine &engine) {
    int i = 1;
    while (i < argc) {
        const char *arg1 = argv[i];
        const char *arg2 = (i == argc - 1) ? "" : argv[i + 1];
        i += handleFlag(arg1, arg2, engine);
    }
}

void printHelp() {
    std::cout << "Usage: ./projectchimera [-l <level-name>]" << std::endl
              << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "  -l <level-name> - load the game directly into the level <level-name>" << std::endl;
}
