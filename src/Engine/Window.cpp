#include "Window.hpp"

Window::Window( int w, int h, RenderSystem *rs_ ) {
	w_width = w;
	w_height = h;
	quit = false;

	rs = rs_;
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
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		// SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

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
				if( !initGL() ) {
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
				SDL_StartTextInput();
			}
		}
	}

	return success;
}

// "Initialize" OpenGL
// Attempts basic operations to test that OpenGL context was properly created
// Returns true if no errors occur, else false
bool Window::initGL() {
	bool success = true;
	GLenum error = GL_NO_ERROR;
	
	error = glGetError();
	if( error != GL_NO_ERROR )
		success = false;

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	error = glGetError();
	if( error != GL_NO_ERROR )
		success = false;
	
	glClearColor( 0, 0, 0, 1 );
	
	error = glGetError();
	if( error != GL_NO_ERROR )
		success = false;
	
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
