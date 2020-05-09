#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <glm/vec2.hpp>

class Window {
private:
	// Control variable for locking the mouse in the window
	bool mouse_lock;

	// The Window and GL context
	SDL_Window *window;
	SDL_GLContext gl_context;

public:
	Window();

	// Initializes the window, OpenGL, and GLEW
	bool init( int w, int h );

	// Stop SDL and destroy the window
	void close();

	// Return a vector of all events that occured in the last tick
	std::vector<SDL_Event> getSDLEvents();

	// Reshape the window
	void reshape( int new_width, int new_height );
	
	// Perform any necessary window operations after rendering
	void postRender();

	// Return the drawable size of the window
	glm::vec2 getDrawableSize();

	// Explicitly set mouse locking
	void setMouseLock( bool new_val );
};

#endif