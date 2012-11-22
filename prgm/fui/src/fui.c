
//
// fui.c
//
// written by sjrct, naitsirhc
//

#include <frosk.h>
#include <shiny/shiny.h>
#include <stdlib.h>

#define SIZE 37

int main()
{

	int x, y;
	pixel_buffer buf;
	
	shiny_thingy * expanse;
	shiny_thingy * buffer;
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
	container_add_thingy(SHINY_CONTAINER(expanse), buffer);
	container_add_thingy(SHINY_CONTAINER(expanse), buffer);

	shiny_main_loop();
	return 0;
}
