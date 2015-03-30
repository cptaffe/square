
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#ifndef WIN_H_
#define WIN_H_

#include <SDL.h>

typedef struct {
	SDL_Window *win;
	SDL_GLContext *gl;
	int height, width; // old height and width
} win;

// construct & destruct.
int win_init(win *w);
void win_destroy(win *w);

void win_draw(win *w);
int win_loop(win *w);
int win_event(win *w, SDL_Event *e);

#endif // WIN_H_
