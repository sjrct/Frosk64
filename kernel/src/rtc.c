//
// rtc.c
//
// written by sjrct
//

#include "rtc.h"
#include "inline_asm.h"
#include "interrupt.h"
#include "irq.h"

ulong ticks = 0;

void init_rtc(void)
{
	uchar old, n;
	void rtc_int(void);
	
	register_int(IRQ0 + IRQ_CMOS, (ulong)rtc_int, KERNEL_CS, 0x8E);
	enable_irq(IRQ_CMOS);

	INB(old, 0x70);
	
	OUTB(0x70, 0x8A);
	INB(n, 0x71);
	OUTB(0x70, 0x8A);
	OUTB(0x71, n | 0x40);
	

	OUTB(0x70, old);
}

ulong get_ticks(void)
{
	return ticks;
}
