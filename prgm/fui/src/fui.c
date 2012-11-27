
//
// fui.c
//
// written by sjrct, naitsirhc
//

#include <frosk.h>
#include <shiny/shiny.h>
#include <stdlib.h>

#define SIZE 37

shiny_thingy * buffer;

bool handler_test(shiny_thingy * thingy, event ev) {
	int x, y;
	pixel_buffer buf;
	pixel pxl = { 0, 128, 0 };
	if(ev.u.keyboard.letter == 'a') {
		buf = create_buffer(SIZE, SIZE, pxl);
		for (y = 0; y < SIZE; y++) {
			for (x = 0; x < SIZE; x++) {
				buf.buffer[x][y].r = x % 255;
				buf.buffer[x][y].g = y % 255;
				buf.buffer[x][y].b = (x - y) % 255;
			}
		}
		set_shiny_buffer(SHINY_BUFFER(buffer), buf);
		draw(buffer);
	}
	debug_line("asdf");
	return false;
}

int main()
{
	int x, y;
	pixel_buffer buf;
	
	shiny_thingy * expanse;
	pixel pxl = { 0, 128, 0 };

	buf = create_buffer(SIZE, SIZE, pxl);
	for (y = 0; y < SIZE; y++) {
		for (x = 0; x < SIZE; x++) {
			buf.buffer[x][y].r = x % 255;
			buf.buffer[x][y].g = y % 255;
			buf.buffer[x][y].b = (x + y) % 255;
		}
	}	
	expanse = create_shiny_expanse(150, 50);
	buffer = create_shiny_buffer();

	buffer->size.width = SIZE;
	buffer->size.height = SIZE;
	
	set_shiny_buffer(SHINY_BUFFER(buffer), buf);
	container_add_thingy(SHINY_CONTAINER(expanse), buffer);
	
	register_event_handler(expanse, KEY_DOWN, handler_test);
	register_event_handler(buffer, KEY_DOWN, handler_test);
	
	shiny_main_loop();
	return 0;
}
