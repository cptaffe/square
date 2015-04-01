
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <win.h>

#include <stdexcept>

win::win() {
	// init artist
	art = new picaso(new brain());
	// add subject to art
	art->subject((drawable *) new square());
	// paint for first time
	art->paint();
}

// cleanup work
win::~win() {
	delete art->getMind();
	delete art;
}

int win::event(SDL_Event *e) {
	if (SDL_PollEvent(e)) {

		// handle event
		if (e->type == SDL_QUIT) {
			return 1;
		} else if (e->type == SDL_WINDOWEVENT) {
			if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
				art->resize(e->window.data1, e->window.data2); // update
			}
		}

		SDL_GL_SwapWindow(window);
	}
	return 0;
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

brain::brain() {

	puts("brain init");

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

	reset(); // setup gl
}

brain::~brain() {
	puts("brain destroyed");
	SDL_GL_DeleteContext(gl);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Window *brain::getWindow() {
	return window;
}

SDL_GLContext brain::getGLContext() {
	return gl;
}

void brain::reset() {}

// call on initial draw to set up gl
picaso::picaso(mind *state) {
	this->state = state;

	// set clear color (background color)
	glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);

	// depth
	glEnable(GL_DEPTH_TEST);
}

picaso::~picaso() {
	// delete drawables and vector
	for (auto i = subjects.begin(); i != subjects.end(); i++) {
		delete *i; // drawable
	}
}

void picaso::resize(int width, int height) {
	// set viewport size to current window size.
	glViewport(0, 0, width, height);

	state->reset();
	paint();
}

void picaso::paint() {

	// clear to new clear color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto i = subjects.begin(); i != subjects.end(); i++) {
		(*i)->draw(); // drawable
	}

	SDL_Window *w = state->getWindow();
	SDL_GL_SwapWindow(w);
}

void picaso::subject(drawable *d) {
	subjects.push_back(d);
}

mind *picaso::getMind() {
	return state;
}

triangle::triangle(std::vector<GLfloat> *vertices) {
	this->vertices = vertices;

	GLfloat *v = vertices->data();
	for (int i = 0; i < 9; i++) {
		printf("float: %f\n", v[i]);
	}
}

triangle::~triangle() {
	delete vertices;
}

void triangle::draw() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices->data());

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDisableVertexAttribArray(0);
}

square::square() {
	const GLfloat triangle1[] = {
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	const GLfloat triangle2[] = {
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	triangles.push_back(new triangle(new std::vector<GLfloat>(std::begin(triangle1), std::end(triangle1))));
	triangles.push_back(new triangle(new std::vector<GLfloat>(std::begin(triangle2), std::end(triangle2))));
}
square::~square() {
	for (auto i = triangles.begin(); i != triangles.end(); i++) {
		delete *i;
	}
}

void square::draw() {
	// draws triangles
	for (auto i = triangles.begin(); i != triangles.end(); i++) {
		(*i)->draw(); // drawable
	}
}
