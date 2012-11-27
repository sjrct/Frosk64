#include "bearing_intrn.h"
#include <debug.h>

int main() {
	debug_line("Hello World!");
	if(!register_em()) {
		return 1;
	}	

	while(1) {
		serve();
	}
	return 0;
}

// from handle event or expanse system
void resize_expanse(expanse e, int new_width, int new_height) {
	e.api_exp.width = new_width;
	e.api_exp.height = new_height;
//	set_expanse_geometry(&e);
	update_expanse(e);
//	update_decoration_buffer(e);
}

// from handle event?
void move_expanse(expanse e, int new_x, int new_y) {
	e.x = new_x;
	e.y = new_y;
	update_expanse(e);
}
