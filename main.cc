
// Copyright (c) 2015 Connor Taffe. All rights reserved.

#include <stdio.h>
#include <stdexcept>

#include <win.h>

int main(int argc, char *argv[]) {
	win w;
	int err;

	try {
		w.run();
	} catch (std::runtime_error e) {
		puts(e.what());
		return 1;
	}

	return 0;
}
