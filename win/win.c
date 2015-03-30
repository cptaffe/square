
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <win.h>
#include <SDL_opengles2.h>

int win_init(win *w) {
	int posX = 100, posY = 100;
	w->width = 320;
	w->height = 240;
	int wflags = SDL_WINDOW_OPENGL |  SDL_WINDOW_RESIZABLE;

	SDL_Init(SDL_INIT_VIDEO);

	w->win = SDL_CreateWindow("Hello World", posX, posY, w->width, w->height, wflags);
	if (w->win == NULL) {
		fputs("gl context creation failed.", stderr);
		return 1;
	}

	w->gl = SDL_GL_CreateContext(w->win);
	if (w->gl == NULL) {
		fputs("gl context creation failed.", stderr);
		win_destroy(w);
		return 1;
	}

	// print version
	const unsigned char *str = glGetString(GL_VERSION);
	printf("version: %s", str);

	win_draw(w);

	return 0;
}

// cleanup work
void win_destroy(win *w) {
	SDL_GL_DeleteContext(w->gl);
	SDL_DestroyWindow(w->win);
	SDL_Quit();
}

int win_event(win *w, SDL_Event *e) {
	if (SDL_PollEvent(e)) {

		// handle event
		if (e->type == SDL_QUIT) {
			return e->type;
		}
		SDL_GL_SwapWindow(w->win);
	}
	return 0;
}

void win_draw(win *w) {
	const double vertexes[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	int i;
	size_t len = (sizeof(vertexes) / (sizeof(double) * 3));

	glPushMatrix();
	glBegin(GL_POLYGON);

	for (i = 0; i < len; i++) {
		const double *vertex = &vertexes[i];
		glVertex3d(vertex[0], vertex[1], vertex[2]);
	}

	glEnd();
	glPopMatrix();

}

// event loop
int win_loop(win *w) {
	for (;;) {
		SDL_Event e;
		int err;

		if ((err = win_event(w, &e)) != 0) {
			puts("event errored!");
			return err;
		}

		if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				int height, width;
				double wr, hr; // ratios

				SDL_GetWindowSize(w->win, &width, &height);

				wr = ((double) w->width)/width;
				hr = ((double) w->height)/height;

				printf("from %d x %d to %d x %d\n"
				"width ratio = %lf, height ratio = %lf\n",
				w->width, w->height, width, height, wr, hr);

				w->width = width;
				w->height = height;
				win_draw(w);
			} else {
				// printf("unknown event type %d\n", e.window.event);
			}
		}
	}
}
