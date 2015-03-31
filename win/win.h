
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#ifndef WIN_H_
#define WIN_H_

#include <SDL.h>

#include <vector>

// Drawable interface
class drawable {
public:
	virtual void draw() = 0;
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
};

// our artist is named picaso.
class picaso : public artist {
protected:
	std::vector<drawable *> subjects;
	SDL_Window *window;
	int width, height;
public:
	picaso(SDL_Window *window);
	~picaso();

	// implement artist
	virtual void paint();
	virtual void resize(int width, int height);
	virtual void subject(drawable *d);
};

class square : public drawable {
public:
	square();
	~square();

	// implement renderable
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
