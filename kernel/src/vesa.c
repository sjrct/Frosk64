//
// vesa.c
//
// written by sjrct
//

#include "vesa.h"
#include "kerndef.h"
#include "inline_asm.h"
#include "paging.h"
#include "pagemgr.h"
#include <bufferutils.h>

#define GR_ADDR_REG 0x3CE

// graphics controller regs
#define SET_RESET    0
#define EN_SET_RESET 1
#define DATA_ROT     3
#define GR_MODE      5
#define BIT_MASK     8

video_mode_info * cur_vmi = (video_mode_info*)VM_INFO_LOC;
static ulong virt_addr = 0;

void init_vesa(void)
{
	ulong size = cur_vmi->yres * cur_vmi->ppsl;
	if (size % 0x1000) size = (size & ~0xFFF) + 0x1000;
	virt_addr = KSPACE_LOC + 0x100000;	// FIXME
//	virt_addr = alloc_pages(size / 0x1000, KVIRT_PAGES);
	page_in(cur_vmi->phys_base_ptr, virt_addr, size, 3);
}

void vesa_draw(char * rect, int sx, int sy, int w, int h)
{
	ulong addr;
	int x, y, k, i = 0;
	int mx = cur_vmi->bpp / 8;
	int my = cur_vmi->ppsl;
		
	for (y = sy; y < sy + h; y++) {
		for (x = sx; x < sx + w; x++) {
			addr = virt_addr + x * mx + y * my;
			for (k = 0; k < mx; k++) {
				ATB(addr + k) = rect[i++];
			}
		}
	}
}

void vesa_fill(pixel * c, int sx, int sy, int w, int h)
{
	ulong addr;
	int x, y;
	int mx = cur_vmi->bpp / 8;
	int my = cur_vmi->ppsl;
		
	for (y = sy; y < sy + h; y++) {
		for (x = sx; x < sx + w; x++) {
			addr = virt_addr + x * mx + y * my;
			ATB(addr + 0) = c->r;
			ATB(addr + 1) = c->g;
			ATB(addr + 2) = c->b;
		}
	}
}
/*
{

	// TODO remove page switches from this function
	//	(it would be faster and no potential data loss)

	ulong addr, base, offset;
	ulong pbase = cur_vmi->phys_base_ptr;
	int x, y, k, i = 0;
	int mx = cur_vmi->bpp / 8;
	int my = cur_vmi->ppsl;
	
	for (y = sy; y < sy + h; y++) {
		for (x = sx; x < sx + w; x++) {
			addr = pbase + x * mx + y * my;
			offset = addr % 0x1000;
			base = addr - offset;
			
			if (ATQ(KPT0_0_LOC) != (base | 3)) {
				ATQ(KPT0_0_LOC) = base | 3;
				INVLPG(KSPACE_LOC);
			}
			
			for (k = 0; k < mx; k++) {
				ATB(KSPACE_LOC + offset + k) = rect[i++];
			}
		}
	}
}*/

gr_info * vesa_get_info(void)
{
	static gr_info inf;
	inf.resx = cur_vmi->xres;
	inf.resy = cur_vmi->yres;
	inf.bpp = cur_vmi->bpp;
	inf.bpsl = cur_vmi->ppsl;
	return &inf;
}
