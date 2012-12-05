//
// shiny_buffer.c
//
// written by naitsirhc
//

#include <stdlib.h>
#include <shiny/shiny.h>
#include <shiny/shiny_buffer.h>

struct shiny_buffer{
	pixel_buffer buffer;
};

shiny_thingy * create_shiny_buffer(expanse_handle h) {
	shiny_thingy * thingy;
	thingy = malloc(sizeof(shiny_thingy));
	SHINY_BUFFER(thingy) = malloc(sizeof(shiny_buffer));
	thingy->loc.expanse_handle = h;
	thingy->type = BUFFER;
	return thingy;
}

void draw_shiny_buffer(shiny_buffer * buffer, shiny_loc loc, shiny_size size) {
//	resize_buffer(buffer->buffer, size.width, size.height);
	draw_buffer(buffer->buffer, loc);
}

void set_shiny_buffer(shiny_thingy * thingy, pixel_buffer buff) {
	shiny_buffer * thing = SHINY_BUFFER(thingy);
	free_buffer(thing->buffer);
	thing->buffer = copy_buffer(buff);
	thingy->size.width = buff.width;
	thingy->size.height = buff.height;
}
