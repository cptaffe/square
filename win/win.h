
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#ifndef WIN_H_
#define WIN_H_

#include <vector>

#include <SDL.h>
#include <SDL_opengles2.h>

// Drawable interface
class drawable {
public:
	virtual void draw() = 0;
};

// state of rendering interface
class mind {
public:
	virtual SDL_Window *getWindow() = 0;
	virtual SDL_GLContext getGLContext() = 0;
	virtual void reset() = 0; // call on gl context trashing
	virtual void swap() = 0;
};

// artist interface (manages drawing)
class artist {
public:
	// paint all drawables
	virtual void paint() = 0;
	// resize canvas
	virtual void resize(int width, int height) = 0;
	// new drawable subject
	virtual void subject(drawable *d) = 0;
	// get mindset (state)
	virtual mind *getMind() = 0;
};

// our mindset (rendering state) is called a brain.
class brain : public mind {
protected:
	SDL_Window *window;
	SDL_GLContext gl;
public:
	brain();
	~brain();

	// implement mind
	virtual SDL_Window *getWindow();
	virtual SDL_GLContext getGLContext();
	virtual void reset();
	virtual void swap();
};

// our artist is named picaso.
class picaso : public artist {
protected:
	std::vector<drawable *> subjects;
	mind *state;
public:
	picaso(mind *m);
	~picaso();

	// implement artist
	virtual void paint();
	virtual void resize(int width, int height);
	virtual void subject(drawable *d);
	virtual mind *getMind();
};

// gl vertex
class point {
public:
	GLfloat x, y, z;

	point(GLfloat x, GLfloat y, GLfloat z);
	~point();

	void appendto(std::vector<GLfloat> *v);
};

class triangle : public drawable {
protected:
	std::vector<point *> points;
	std::vector<GLfloat> vertices;

	void vectorize_points();
public:
	triangle(point *p1, point *p2, point *p3);
	~triangle();

	// implement drawable
	virtual void draw();
};

class square : public drawable {
protected:
	std::vector<drawable *> triangles;
public:
	square();
	~square();

	// implement drawable
	virtual void draw();
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
	~win();

	void run(); // runs event loop
}; // win

#endif // WIN_H_
