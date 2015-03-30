
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <win.h>
#include <SDL_opengles2.h>

#include <stdexcept>

win::win() {
	int posX = 100, posY = 100, width = 512, height = 512;
	int wflags = SDL_WINDOW_OPENGL |  SDL_WINDOW_RESIZABLE;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Square", posX, posY, width, height, wflags);
	if (window == NULL) {
		throw new std::runtime_error("window creation failed.");
	}

	gl = SDL_GL_CreateContext(window);
	if (gl == NULL) {
		throw new std::runtime_error("gl context creation failed.");
	}

	// print version
	const unsigned char *str = glGetString(GL_VERSION);
	printf("version: %s\n", str);

	draw = new win::drawing(window); // init draw
}

// cleanup work
win::~win() {
	SDL_GL_DeleteContext(gl);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int win::event(SDL_Event *e) {
	if (SDL_PollEvent(e)) {

		// handle event
		if (e->type == SDL_QUIT) {
			return 1;
		} else if (e->type == SDL_WINDOWEVENT) {
			if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
				draw->resize(e->window.data1, e->window.data2); // update
			}
		}

		SDL_GL_SwapWindow(window);
	}
	return 0;
}

// call on initial draw to set up gl
win::drawing::drawing(SDL_Window *window) {

	win = window;
	SDL_GetWindowSize(win, &height, &width);

	// set clear color (background color)
	glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);

	// depth
	glEnable(GL_DEPTH_TEST);

	render();
}

void win::drawing::resize(int width, int height) {
	this->width = width;
	this->height = height;

	// set viewport size to current window size.
	glViewport(0, 0, width, height);

	render();
}

void win::drawing::render() {

	// current triangle buffer
	GLfloat vertices[] = {

		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

	};

	// clear to new clear color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);

	SDL_GL_SwapWindow(win);
}

// event loop
void win::run() {
	for (;;) {
		SDL_Event e;

		if (event(&e) != 0) {
			return; // it's over man.
		}
	}
}
