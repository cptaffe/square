
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <stdexcept>

#include <win.h>

win::win() : art(new picaso(new brain())) {
	// add subject to art
	art->subject((drawable *) new square());
	// paint for first time
	art->paint();
}

// cleanup work
win::~win() {
	delete art->state();
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

point::point(GLfloat x, GLfloat y, GLfloat z)
	: x(x), y(y), z(z) {}

point::~point() {}

void point::appendto(std::vector<GLfloat> *v) {
	v->push_back(x);
	v->push_back(y);
	v->push_back(z);
}

brain::brain() {
	int posX = 100, posY = 100, width = 512, height = 512;
	int wflags = SDL_WINDOW_OPENGL |  SDL_WINDOW_RESIZABLE;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	window_ = SDL_CreateWindow("Square", posX, posY, width, height, wflags);
	if (window_ == NULL) {
		throw new std::runtime_error("window creation failed.");
	}

	gl_context_ = SDL_GL_CreateContext(window_);
	if (gl_context_ == NULL) {
		throw new std::runtime_error("gl context creation failed.");
	}
}

brain::~brain() {
	SDL_GL_DeleteContext(gl_context_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void brain::swap() {
	SDL_GL_SwapWindow(window_);
}

SDL_Window *brain::window() const { return window_; }

SDL_GLContext brain::gl_context() const { return gl_context_; }

timer::timer(uint32_t delay, uint32_t(*func)(uint32_t, void *), void *param)
	: timer_(SDL_AddTimer(delay, func, param)) {
		if (timer_ == 0) {
			// bad timer id
			throw new std::runtime_error("timer addition failed.");
		}
	}

timer::~timer() {
	if (SDL_RemoveTimer(timer_) == SDL_FALSE) {
		// FIXME: seems to always fail.
		// throw new std::runtime_error("timer removal failed.");
	}
}

// timer callback
namespace {
extern "C"{

uint32_t t_callback(uint32_t interval, void *obj) {
	try {
		uint32_t ninterval = static_cast<artist *>(obj)->timer_callback(interval);
		return ninterval;
	} catch (...) {
		return 0; // no new timer interval because of error.
	}
}

} // namespace
} // extern "C"

// call on initial draw to set up gl
picaso::picaso(mind *m) : state_(m) {
	// set up sdl timer
	timers.push_back(new timer(33, &t_callback, this));

	// set clear color (background color)
	glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
}

picaso::~picaso() {
	// delete timers
	for (auto i = timers.begin(); i != timers.end(); i++) {
		delete *i; // timer
	}

	// delete drawables and vector
	for (auto i = subjects.begin(); i != subjects.end(); i++) {
		delete *i; // drawable
	}
}

uint32_t picaso::timer_callback(uint32_t interval) {
	static int a = 0;
	printf("test!: %d\n", a++);
	return interval; // keep interval
}

void picaso::resize(int width, int height) {
	// set viewport size to current window size.
	glViewport(0, 0, width, height);

	paint();
}

void picaso::paint() {
	// clear to new clear color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto i = subjects.begin(); i != subjects.end(); i++) {
		(*i)->draw(); // drawable
	}

	state_->swap(); // swap gl buffer with window buffer
}

void picaso::subject(drawable *d) {
	subjects.push_back(d);
}

mind *picaso::state() const { return state_; }

triangle::triangle(point *p1, point *p2, point *p3) {
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	vectorize_points();
}

triangle::~triangle() {
	for (auto i = points.begin(); i != points.end(); i++) {
		delete *i; // delete
	}
}

void triangle::vectorize_points() {
	// push into vector
	for (auto i = points.begin(); i != points.end(); i++) {
		(*i)->appendto(&vertices); // append into vertices.
	}
}

void triangle::draw() {

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
