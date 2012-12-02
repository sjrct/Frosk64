//
// mouse.c
//
// written by sjrct
//

#include "mouse.h"
#include "interrupt.h"
#include "irq.h"
#include "kerndef.h"
#include "inline_asm.h"
#include "keyboard.h"

int mousex = 0, mousey = 0;

void init_mouse(void)
{
	void mouse_move(void);
	void init_mouse_asm(void);

	init_mouse_asm();

	register_int(IRQ0 + IRQ_PS2_MOUSE, (ulong)mouse_move, KERNEL_CS, 0x8E);
	enable_irq(IRQ_PS2_MOUSE);
}

long get_mouse_pos(int * x, int * y)
{
	uchar stat;
	int i;
	long r;
	signed char mx, my;
	uchar btns, mask;
	char w;
	static uchar obtns = 0;
	
	INB(stat, 0x64);

	if (stat & 1) {
		INB(btns, 0x60);
		INB(stat, 0x64);
		if (!(stat & 1) || !(btns & 0x8)) {
			kb_write((char*)&btns, 1);
			return 0;
		}

		INB(mx, 0x60);
		INB(stat, 0x64);
		if (!(stat & 1)) {
			kb_write((char*)&btns, 1);
			kb_write((char*)&mx, 1);
			return 0;
		}

		INB(my, 0x60);
		
		mousex += (int)mx;
		mousey -= (int)my;

		for (i = 0; i <= 2; i++) {
			mask = 1 << i;
			if ((btns & mask) != (obtns & mask)) {
				if (btns & mask) w = 0x70 + i;
				else w = 0xf0 + i;
				kb_write(&w, 1);
			}
		}
		
		obtns = btns;
		r = 1;
	} else {
		r = 0;
	}

	*x = mousex;
	*y = mousey;
	return r;
}
