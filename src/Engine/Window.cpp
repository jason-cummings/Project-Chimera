#include "Window.hpp"

Window::Window( int w, int h ) {
	w_width = w;
	w_height = h;
	quit = false;
	rs = nullptr;
}

bool Window::getQuit() { return quit; }

// Initialize the SDL window and OpenGL and return success
bool Window::init() {
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		// Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		
		window = SDL_CreateWindow( "Project Chimera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ); // SDL_WINDOW_FULLSCREEN_DESKTOP
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
				
				SDL_StartTextInput();
			}
		}
	}

	return success;
}

void Window::handleKeys(SDL_Event e) {
	if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q )
		quit = true;
}

void Window::close() {
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

// Test for any window events and handle them appropriately
void Window::handleEvents() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 ) {
		if( e.type == SDL_QUIT )
			quit = true;
		else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
			w_width = e.window.data1;
			w_height = e.window.data2;
			if( rs != nullptr )
				rs->reshape( w_width, w_height );
		}
		else if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ) {
			handleKeys(e);
		}
		// else if( e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP ||  e.type == SDL_MOUSEMOTION ) {
		//     handleMouse( e );
		// }
	}
}

// Perform any necessary operations after rendering
void Window::postRender() {
	SDL_GL_SwapWindow( window );
}

void Window::setRS( RenderSystem *rs_ ) {
	rs = rs_;
}

glm::vec2 Window::getDisplaySize() {
	int w, h;
	SDL_GL_GetDrawableSize( window, &w, &h );
	return glm::vec2( w, h );
}
