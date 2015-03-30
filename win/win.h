
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#ifndef WIN_H_
#define WIN_H_

#include <SDL.h>

class win {
protected:
	SDL_Window *window;
	SDL_GLContext gl;

	class drawing {
	protected:
		SDL_Window *win;
		int height, width; // old height and width
	public:
		drawing(SDL_Window *win);
		void render();
		void resize(int width, int height);
	} *draw;

	int event(SDL_Event *e); // handles an event

public:
	win();
	~win();

	void run(); // runs event loop
}; // win

#endif // WIN_H_
