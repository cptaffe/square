
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#ifndef WIN_H_
#define WIN_H_

#include <vector>

#include <SDL.h>
#include <SDL_opengles2.h>

// timer wrapper
class timer {
	SDL_TimerID t;
public:
	timer(uint32_t delay, uint32_t(*func)(uint32_t, void *), void *param);
	~timer();
};

// Drawable interface
class drawable {
public:
	virtual ~drawable() {};
	virtual void draw() = 0;
};

// state of rendering interface
class mind {
public:
	virtual ~mind() {};
	virtual SDL_Window *window() const = 0;
	virtual SDL_GLContext gl_context() const = 0;
	virtual void swap() = 0;
};

// artist interface (manages drawing)
class artist {
public:
	virtual ~artist() {};
	// paint all drawables
	virtual void paint() = 0;
	// resize canvas
	virtual void resize(int width, int height) = 0;
	// new drawable subject
	virtual void subject(drawable *d) = 0;
	// get mindset (state)
	virtual mind *state() const = 0;
	// callback for timers
	virtual uint32_t timer_callback(uint32_t interval) = 0;
};

// our mindset (rendering state) is called a brain.
class brain : public mind {
public:
	brain();
	virtual ~brain();

	// implement mind
	virtual SDL_Window *window() const;
	virtual SDL_GLContext gl_context() const;
	virtual void swap();
private:
	SDL_Window *window_;
	SDL_GLContext gl_context_;
};

// our artist is named picaso.
class picaso : public artist {
public:
	picaso(mind *m);
	virtual ~picaso();

	// implement artist
	virtual void paint();
	virtual void resize(int width, int height);
	virtual void subject(drawable *d);
	virtual mind *state() const;
	virtual uint32_t timer_callback(uint32_t interval);
private:
	std::vector<drawable *> subjects;
	std::vector<timer *> timers;
	mind *state_;
};

// gl vertex
class point {
public:
	GLfloat x, y, z;

	point(GLfloat x, GLfloat y, GLfloat z);
	virtual ~point();

	void appendto(std::vector<GLfloat> *v);
};

class triangle : public drawable {
public:
	triangle(point *p1, point *p2, point *p3);
	virtual ~triangle();

	// implement drawable
	virtual void draw();
private:
	std::vector<point *> points;
	std::vector<GLfloat> vertices;

	void vectorize_points();
};

class square : public drawable {
public:
	square();
	virtual ~square();

	// implement drawable
	virtual void draw();
private:
	std::vector<drawable *> triangles;
};

class win {
protected:
	SDL_Window *window;
	SDL_GLContext gl;

	// manages drawing
	artist *art;

	int event(SDL_Event *e); // handles an event

public:
	win();
	virtual ~win();

	void run(); // runs event loop
}; // win

#endif // WIN_H_
