//
// tga.c
//
// written by sjrct
//

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tga.h"

#define NODATA      0
#define COLORMAPPED 1
#define TRUECOLOR   2
#define MONOCHROME  3
#define ENCODED     8

#pragma pack(push, 1)
typedef struct {
	uchar  identsz;
	uchar  col_map_type;
	uchar  img_type;
	ushort col_map_start;
	ushort col_map_len;
	uchar  col_map_bits;
	ushort xstart;
	ushort ystart;
	ushort width;
	ushort height;
	uchar  bpp;
	uchar  desc;
} tgah;
#pragma pack(pop)



pixel_buffer from_linear_buffer(const char * lbuf, int x, int y) {
	pixel_buffer pb;
	pixel p = {0,0,0};
	pb = create_buffer(x,y,p);
	int i,j;
	for(j = 0; j < y; j++) {
		for(i = 0; i < x; i++) {
			pb.buffer[y,x].r = lbuf[y*x*4+0];
			pb.buffer[y,x].g = lbuf[y*x*4+1];
			pb.buffer[y,x].b = lbuf[y*x*4+2];
			//ignore alpha
		}
	}
	return pb;
}

int parse_tga(const char * buf, uint sz, tga_image * to)
{
	pixel pixel = {0,0,0};
	uchar len;
	ulong i, j, k, l, size;
	tgah * hdr;
	
	hdr = (tgah*)buf;
	to->bpp = hdr->bpp;
	to->w = hdr->width;
	to->h = hdr->height;
	buf += sizeof(tgah) + hdr->identsz;
	
	size = to->w * to->h * to->bpp;
	
	switch (hdr->img_type) {
		case TRUECOLOR:
			if (to->col_map_type) return false;
			to->buffer = from_linear_buffer(buf, x, y);
			break;
		/*
		case TRUECOLOR | ENCODED:
			if (to->col_map_type) return false;
		
			for (i = l = 0; l < size;) {
				len = buf[i++];
			
				if (len & 0x80) {
					for (k = 0; k < to->bpp; k++) {
						pixel[k] = buf[i++];
					}
				
					for (j = 0; j < len - 127; j++) {
						for (k = 0; k < to->bpp; k++) {
							to->data[l++] = pixel[k];
						}
					}
				} else {
					for (j = 0; j < len + 1; j++) {
						for (k = 0; k < to->bpp; k++) {
							to->data[l++] = buf[i++];
						}
					}
				}
			}
			break;
		*/
	default:
		return false;
	}
	
	return true;
}

