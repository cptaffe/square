
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <stdexcept>

#include <win.h>

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

point::point(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
point::~point() {}

void point::appendto(std::vector<GLfloat> *v) {
	v->push_back(x);
	v->push_back(y);
	v->push_back(z);
}

brain::brain() {

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

void brain::swap() {
	SDL_GL_SwapWindow(window);
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

	state->swap(); // swap gl buffer with window buffer
}

void picaso::subject(drawable *d) {
	subjects.push_back(d);
}

mind *picaso::getMind() {
	return state;
}

triangle::triangle(point *p1, point *p2, point *p3) {
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	vectorize_points();
}

void triangle::vectorize_points() {
	// push into vector
	for (auto i = points.begin(); i != points.end(); i++) {
		(*i)->appendto(&vertices); // append into vertices.
	}
}

triangle::~triangle() {
	for (auto i = points.begin(); i != points.end(); i++) {
		delete *i; // delete
	}
}

void triangle::draw() {

	for (auto i = points.begin(); i != points.end(); i++) {
		printf("(%f, %f, %f), ", (*i)->x, (*i)->y, (*i)->z);
	}
	puts("drawing!");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

square::square() {

	triangles.push_back(
		new triangle(
			new point(-0.5f, -0.5f, 0.0f),
			new point(0.5f, 0.5f, 0.0f),
			new point(-0.5f, 0.5f, 0.0f)
		)
	);

	triangles.push_back(
		new triangle(
			new point(0.5f, 0.5f, 0.0f),
			new point(-0.5f, -0.5f, 0.0f),
			new point(0.5f, -0.5f, 0.0f)
		)
	);
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
