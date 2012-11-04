//
// mouse.c
//
// written by sjrct
//

#include "mouse.h"
#include "interrupt.h"
#include "irq.h"
#include "kerndef.h"

int mousex = 0, mousey = 0;

void init_mouse(void)
{
	void mouse_move(void);
	void init_mouse_asm(void);
	
	register_int(IRQ0 + IRQ_PS2_MOUSE, (ulong)mouse_move, KERNEL_CS, 0x8F);
	enable_irq(IRQ_PS2_MOUSE);
	init_mouse_asm();
}

void get_mouse_pos(int * x, int * y)
{
	*x = mousex;
	*y = mousey;
}