#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

class Window {
private:
	int w_width, w_height;
	bool quit;

	SDL_Window *window;
	SDL_GLContext gl_context;

	bool init();
	bool initGL();
	void testGLError(const char* loc = "default");

public:
	Window( int w, int h );
	bool getQuit();
	void handleKeys(SDL_Event e);
	void close();
	void tick();
	void render();
};

#endif