//
// shiny_thingy.c
//
// written by naitsirhc
//

#include <shiny/shiny_thingy.h>

void draw(shiny_thingy * thingy) {
	switch (thingy->type) {
		case CONTAINER:
			draw_shiny_container(SHINY_CONTAINER(thingy), thingy->loc, thingy->size);
			break;
		case BUFFER:
			draw_shiny_buffer(SHINY_BUFFER(thingy), thingy->loc, thingy->size);
			break;
	}
}
