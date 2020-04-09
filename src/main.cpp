#include <iostream>
#include "Engine/Engine.hpp"

int main( int argc, char *argv[] )
{
	std::cout << "Welcome to Project Chimera" << std::endl;

	std::string level_to_load( "RaptorLand" );
	if( argc > 1 ) {
		level_to_load = argv[1];
	}

	Engine engine = Engine::getEngine();
	
	if( engine.init( level_to_load ) ) {
		while( !engine.getQuit() ) {
			engine.tick();
		}
	}
	else {
		std::cerr << "Could not initialize Engine" << std::endl;
	}

	return 0;
}
