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

int parse_tga(const char * buf, uint sz, tga_image * to)
{
	uchar pixel[4];
	uchar len;
	ulong i, j, k, l, size;
	tgah * hdr;
	
	hdr = (tgah*)buf;
	to->bpp = hdr->bpp;
	to->w = hdr->width;
	to->h = hdr->height;
	buf += sizeof(tgah) + hdr->identsz;
	
	size = to->w * to->h * to->bpp;
	to->data = malloc(size);
	
	switch (hdr->img_type) {
	case TRUECOLOR:
		if (to->col_map_type) return false;
		memcpy(to->data, buf, size);
		break;
	
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
	
	default:
		return false;
	}
	
	return true;
}

