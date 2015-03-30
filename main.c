
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <stdio.h>

#include <win.h>

int main(int argc, char *argv[]) {
	win w;
	int err;

	if (win_init(&w) != 0) {
		puts("win_init failed.");
		return 1;
	}

	if ((err = win_loop(&w)) != 0 && err != SDL_QUIT) {
		puts("event loop encountered an error... closing");
		win_destroy(&w);
		return 1;
	}

	return 0;
}
