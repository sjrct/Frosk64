//
// shiny_thingy.h
//
// written by naitsirhc
//

#ifndef _SHINY_THINGY_H_
#define _SHINY_THINGY_H_

#define SHINY_CONTAINER(X) (X->ptr.container)
#define SHINY_BUFFER(X) (X->ptr.buffer)

#include <expanse.h>
#include <shiny/shiny_buffer.h>
#include <shiny/shiny_container.h>

struct shiny_loc {
	int x, y;
};
struct shiny_size {
	int width, height;
};

struct shiny_thingy {
	shiny_size size;
	shiny_loc loc;
	
	enum {
		CONTAINER = 0,
		BUFFER,
	} type;
	
	union {
		struct shiny_buffer * buffer;
		struct shiny_container * container;
	} ptr;
	
	expanse_handle exp_handle;
};


void draw(shiny_thingy *, shiny_loc);

#endif
