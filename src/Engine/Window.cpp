#include "Window.hpp"

Window::Window( int w, int h ) {
	w_width = w;
	w_height = h;
	quit = false;
	bool success = init();
	if( !success )
		std::cout << "Failure on SDL window initialization" << std::endl;
}

bool Window::getQuit() { return quit; }

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

void Window::tick() {
	SDL_Event e;
	while( SDL_PollEvent(&e) != 0 ) {
		if( e.type == SDL_QUIT )
			quit = true;
		else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
			w_width = e.window.data1;
			w_height = e.window.data2;
			// state->reshape(SCREEN_WIDTH, SCREEN_HEIGHT);
		}
		else if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ) {
			handleKeys(e);
		}
		// else if( e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP ||  e.type == SDL_MOUSEMOTION ) {
		//     handleMouse( e );
		// }
	}

	render();

	SDL_GL_SwapWindow( window );
	testGLError();
}

void Window::render() {
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, w_width, w_height);

	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 1.f);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();

	glDisable(GL_DEPTH_TEST);

	testGLError("render");
}

void Window::testGLError(const char* loc) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cout << "OpenGL error at " << loc << ": " << err << std::endl;
}
