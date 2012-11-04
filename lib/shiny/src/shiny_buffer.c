//
// shiny_buffer.c
//
// written by naitsirhc
//

#include <shiny/shiny.h>
#include <shiny/shiny_buffer.h>

struct shiny_buffer{
	pixel_buffer buffer;
};

void draw_shiny_buffer(shiny_buffer * buffer, shiny_loc loc, shiny_size size) {
	// TODO resize for new size
	resize_buffer(buffer->buffer, size.width, size.height);
	draw_buffer(buffer->buffer, loc);
}

void set_buffer(shiny_buffer * thingy, pixel_buffer buff) {
	free_buffer(thingy->buffer);
	thingy->buffer = copy_buffer(buff);
}
