//
// vesa.h
//
// written by sjrct
//

#ifndef VESA_H
#define VESA_H

#include <cmn/vg.h>
#include "graphics.h"

typedef struct {
	ushort attrib;
	uchar  wina, winb;
	ushort granularity;
	ushort win_size;
	ushort sega, segb;
	uint   winfunc;
	ushort ppsl;
	ushort xres, yres;
	uchar  cwidth, cheight;
	uchar  planes_num;
	uchar  bpp;
	uchar  banks_num;
	uchar  mem_model;
	uchar  bank_size;
	uchar  img_pages_num;
	uchar  resv;
	uchar  red_mask_size;
	uchar  red_pos;
	uchar  green_mask_size;
	uchar  green_pos;
	uchar  blue_mask_size;
	uchar  blue_pos;
	uchar  rsvd_mask_size;
	uchar  rsvd_pos;
	uchar  mode_info;
	uint   phys_base_ptr;
	uint   off_screen_mem_offset;
	ushort off_screen_mem_size;
} video_mode_info;

void init_vesa(void);
void vesa_draw(char *, int, int, int, int);
gr_info * vesa_get_info(void);
//void plot(pixel *);

#endif
