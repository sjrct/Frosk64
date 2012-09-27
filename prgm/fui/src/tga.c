//
// tga.c
//
// written by sjrct
//

#include <frosk.h>
#include <stdlib.h>
#include "tga.h"

void show_tga(const char * fn)
{
	handle_t h;
	long off = 0;
	char fd[0x1000];
	char * sl;
	int x, y, sx, sy;
	
	h = fs_aquire(fn, 0, 1);
	if (!h) return;
	
	sx = *((short*)(fd + 12));
	sy = *((short*)(fd + 14));
	sl = malloc(sx * 3);
		
	fs_release(h);
}
