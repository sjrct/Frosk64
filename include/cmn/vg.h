//
// vg.h
//
// written by sjrct
//

#ifndef _VG_H_
#define _VG_H_

#include <cmn/utypes.h>

typedef struct {
	ushort x, y;
	uint c;
} pixel;

typedef struct {
	int resx, resy;
	int bpp, bpsl;
} gr_info;

#endif
