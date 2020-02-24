#include <iostream>
#include "Engine/Engine.hpp"

int main( int argc, char *argv[] )
{
	std::cout << "Welcome to Project Chimera" << std::endl;

	Engine *engine = Engine::getEngine();
	engine->init();
	while( !engine->getQuit() ) {
		engine->tick( 0.f );
	}

	return 0;
}
