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

    // Create the rendersystem with the window size
    glm::vec2 window_size = window->getDisplaySize();
    rs = new RenderSystem( window_size.x, window_size.y );
    window->setRS(rs);

    return success;
}

void Engine::handleInputs(){
    char inputValue = window->inputs[0].key.keysym.sym;
    if(inputValue == 'w'){
        rs->setTempPH(rs->getTempPH()+2);
    }
    else if(inputValue == 's'){
        rs->setTempPH(rs->getTempPH()-2);
    }
    else if(inputValue == 'a'){
        rs->setTempTH(rs->getTempTH()+2);
    }
    else if(inputValue == 'd'){
        rs->setTempTH(rs->getTempTH()-2);
    }
    std::cout<<"TempPh = " << rs->getTempPH() <<", TempTh = "<< rs->getTempTH()<<std::endl;


    window->inputs.erase(window->inputs.begin());
    if(window->inputs.empty()){
        window->buttonPressed = false;
    }
}


void Engine::tick( double dt ) {
    // Test for input events
    window->handleEvents();
    if(window->buttonPressed){
        handleInputs();
    }

    // Render all
    rs->render(dt);
    // Tell the window to handle any post rendering necessicities
    window->postRender();
}
