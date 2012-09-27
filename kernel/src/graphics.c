//
// graphics.c
//
// written by sjrct
//

#include "vesa.h"

static void (*draw)(char *, int, int, int, int);
static gr_info * (*get_info)(void);

void init_graphics(void)
{
	// TODO detection (eventually)
	
	init_vesa();
	draw = vesa_draw;
	get_info = vesa_get_info;
}

void gr_draw(char * b, int x, int y, int w, int h)
{
	draw(b, x, y, w, h);
}

gr_info * gr_get_info(void)
{
	return get_info();
}
