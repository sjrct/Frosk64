
//
// fui.c
//
// written by sjrct, naitsirhc
//

#include <frosk.h>
#include <shiny/shiny.h>
#include <stdlib.h>
#include "debug.h"

#define SIZE 37

shiny_thingy * buffer;
pixel_buffer norm;
pixel_buffer fill;
pixel_buffer select;

void init_buffers() {
	int x,y;
	pixel pxl = { 0, 128, 0 };
	
	norm = create_buffer(SIZE, SIZE, pxl);
	fill = create_buffer(SIZE, SIZE, pxl);
	select = create_buffer(SIZE, SIZE, pxl);
	
	for (y = 0; y < SIZE; y++) {
		for (x = 0; x < SIZE; x++) {
			fill.buffer[x][y].r = x % 255;
			fill.buffer[x][y].g = y % 255;
			fill.buffer[x][y].b = (x - y) % 255;
		}
	}
	
	for (y = 0; y < SIZE; y++) {
		for (x = 0; x < SIZE; x++) {
			norm.buffer[x][y].r = x % 255;
			norm.buffer[x][y].g = y % 255;
			norm.buffer[x][y].b = (x + y) % 255;
		}
	}
	
	for (y = 0; y < SIZE; y++) {
		for (x = 0; x < SIZE; x++) {
			select.buffer[x][y].r = x % 255 - 50;
			select.buffer[x][y].g = y % 255 - 50;
			select.buffer[x][y].b = (x + y) % 255;
		}
	}
}

bool select_test(shiny_thingy * thingy, event ev) {
	static bool selected = false;
	selected = !selected;
	
	set_shiny_buffer(thingy, selected ? select : norm);
	draw(thingy);
	return true;
}

bool handler_test(shiny_thingy * thingy, event ev) {
	if(ev.u.keyboard.letter == 'a') {
		set_shiny_buffer(buffer, fill);
		draw(buffer);
	}
	return false;
}

int main()
{
	shiny_thingy * expanse;
	shiny_thingy * expanse2;
	shiny_thingy * buffer2;

	init_buffers();

	expanse = create_shiny_expanse(150, 50);
	expanse2 = create_shiny_expanse(100, 100);
	buffer = create_shiny_buffer(expanse->loc.expanse_handle);
	buffer2 = create_shiny_buffer(expanse2->loc.expanse_handle);
	
	set_shiny_buffer(buffer, norm);
	set_shiny_buffer(buffer2, fill);
	container_add_thingy(SHINY_CONTAINER(expanse), buffer);
	container_add_thingy(SHINY_CONTAINER(expanse2), buffer2);
	
//	register_event_handler(expanse, KEY_DOWN, handler_test);
	register_event_handler(buffer, KEY_DOWN, handler_test);
//	register_event_handler(buffer, MOUSE_DOWN, select_test);
	register_event_handler(buffer2, MOUSE_UP, select_test);
	
	shiny_main_loop();
	return 0;
}
