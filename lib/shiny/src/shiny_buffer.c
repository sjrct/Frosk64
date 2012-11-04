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

shiny_thingy * create_shiny_buffer() {
	shiny_thingy * thingy;
	thingy = malloc(sizeof(shiny_thingy));
	SHINY_BUFFER(thingy) = malloc(sizeof(shiny_buffer));
	return thingy;
}

void draw_shiny_buffer(shiny_buffer * buffer, shiny_loc loc, shiny_size size) {
	resize_buffer(buffer->buffer, size.width, size.height);
	draw_buffer(buffer->buffer, loc);
}

void set_shiny_buffer(shiny_buffer * thingy, pixel_buffer buff) {
	free_buffer(thingy->buffer);
	thingy->buffer = copy_buffer(buff);
}
