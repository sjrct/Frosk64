//
// fui.c
//
// written by sjrct, naitsirhc
//

#include <shiny/shiny.h>

int main()
{
	int x, y;
	pixel_buffer buf;
	
	shiny_thingy * expanse;
	shiny_thingy * buffer;
	pixel pxl = { 0, 128, 0 };
	
	expanse = create_shiny_expanse(150, 150);
	buffer = create_shiny_buffer();
	
	buf = create_buffer(100, 100, pxl);
	
	for (y = 0; y < 100; y++) {
		for (x = 0; x < 100; x++) {
			buf.buffer[x][y].r = x % 255;
			buf.buffer[x][y].g = y % 255;
			buf.buffer[x][y].b = (x + y) % 255;
		}
	}
	
	set_shiny_buffer(SHINY_BUFFER(buffer), buf);
	container_add_thingy(SHINY_CONTAINER(expanse), buffer);

/*	for (y = 0; y < 100; y++) {
		for (x = 0; x < 100; x++) {
			buf[x][y][0] = x % 255;
			buf[x][y][1] = (100 - y) % 255;
			buf[x][y][2] = (x + (100 - y)) % 255;
		}
	}*/

//	gr_draw(buf, 120, 20, 100, 100);

	shiny_main_loop();
	return 0;
}
