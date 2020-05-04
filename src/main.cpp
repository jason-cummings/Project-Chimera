#include <iostream>
#include "Engine/Engine.hpp"

int main( int argc, char *argv[] )
{
	std::cout << "Welcome to Project Chimera" << std::endl;

	Engine &engine = Engine::getEngine();
	
	if( engine.init() ) {
		while( !engine.getQuit() ) {
			engine.tick();
		}
	}
	else {
		std::cerr << "Could not initialize Engine" << std::endl;
		return 1;
	}

	return 0;
}
