#include "RenderSystem.hpp"

RenderSystem::RenderSystem( int width, int height ) {
	reshape( width, height );
}

void RenderSystem::reshape( int new_width, int new_height ) {
	view_width = new_width;
	view_height = new_height;
	aspect_ratio = view_width / (float)view_height;
}

void RenderSystem::render( double dt ) {
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, view_width, view_height);

	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 1.f);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();

	glDisable(GL_DEPTH_TEST);
}

void RenderSystem::testGLError( const char *loc ) {
	int err;
	if( (err = glGetError()) != GL_NO_ERROR )
		std::cout << "OpenGL error at " << loc << ": " << err << std::endl;
}
