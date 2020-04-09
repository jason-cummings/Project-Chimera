#include "Window.hpp"

Window::Window() {}

// Initialize the SDL window and OpenGL and return success
bool Window::init( int w_width, int w_height ) {
	bool success = true;
	mouse_lock = false;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		// Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		
		window = SDL_CreateWindow( "Project Chimera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP ); // SDL_WINDOW_FULLSCREEN_DESKTOP or SDL_WINDOW_RESIZABLE
		if( window == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			gl_context = SDL_GL_CreateContext( window );
			if( gl_context == NULL ) {
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				
				// Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewstatus = glewInit();
				if( glewstatus != GLEW_OK ) {
					std::cerr << "GLEW failed to initialize: " << glewGetErrorString(glewstatus) << std::endl;
				}
				
				// Start receiving keyboard input
				SDL_StartTextInput();

				// Turn on mouse lock by default
				toggleMouseLock();
			}
		}
	}

	return success;
}

// Quit SDL and destroy the window
void Window::close() {
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

// Return all events that occurred in the last tick
std::vector<SDL_Event> Window::getSDLEvents() {
	std::vector<SDL_Event> events;
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 ) {
		events.push_back(e);
	}
	return events;
}

// Perform any necessary operations after rendering
void Window::postRender() {
	SDL_GL_SwapWindow( window );
}

// Get the actual drawable size of the window
glm::vec2 Window::getDrawableSize() {
	int w, h;
	SDL_GL_GetDrawableSize( window, &w, &h );
	return glm::vec2( w, h );
}

// Either capture or release the mouse in the window
void Window::toggleMouseLock() {
	mouse_lock = !mouse_lock;
	SDL_SetRelativeMouseMode( mouse_lock ? SDL_TRUE : SDL_FALSE );
}
