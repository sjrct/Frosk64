#include <bufferutils.h>

typedef struct {
	pixel_buffer buffer;
} buffer_thingy;

draw(buffer_thingy * buffer, shiny_loc loc, shiny_size size) {
	// TODO resize for new size
	draw_buffer(buffer, loc);
}

void set_buffer(buffer_thingy * thingy, pixel_buffer buff) {
	free_buffer(thingy);
	thingy->buffer = copy_buffer(buff);
}
