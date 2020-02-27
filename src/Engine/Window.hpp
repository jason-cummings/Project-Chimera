#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <glm/vec2.hpp>

#include "Systems/Rendering/RenderSystem.hpp"

class Window {
private:
	int w_width, w_height;
	bool quit;

	SDL_Window *window;
	SDL_GLContext gl_context;
	RenderSystem *rs; // Should only ever tell the render system to reshape

	void handleKeys( SDL_Event e );

public:
	Window( int w, int h );
	bool init();

	bool getQuit();
	void close();
	void handleEvents();
	void postRender();

	void setRS( RenderSystem *rs_ );

	glm::vec2 getDisplaySize();
};

#endif