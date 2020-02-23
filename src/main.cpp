#include <iostream>
#include "Engine/Window.hpp"

int main( int argc, char *argv[] )
{
	std::cout << "Welcome to Project Chimera" << std::endl;

	Window* window = new Window(640, 480);
	while( !window->getQuit() ) {
		window->tick();
	}

	delete window;
	return 0;
}
