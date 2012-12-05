//
// tga.h
//
// written by sjrct
//

#ifndef TGA_H
#define TGA_H

#include <cmn/utypes.h>
#include <bufferutils.h>

typedef struct {
	uint w, h;
	uchar bpp;
	pixel_buffer buffer;
} tga_image;

int parse_tga(const char *, uint, tga_image *);
void show_tga(tga_image *);

#endif
