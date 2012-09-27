//
// graphics.h
//
// written by sjrct
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cmn/vg.h>

void init_graphics(void);
void gr_draw(char *, int, int, int, int);
gr_info * gr_get_info(void);

#endif
