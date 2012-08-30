//
// irq.h
//
// written by sjrct
//

#ifndef IRQ_H
#define IRQ_H

void init_irqs();
void enable_irq(int);
void disable_irq(int);

enum {
	IRQ_TIMER = 0,
	IRQ_KEYBOARD,
	IRQ_CASCASE,
	IRQ_COM2,
	IRQ_COM1,
	IRQ_LPT2,
	IRQ_FLOPPY,
	IRQ_LPT1,
	
	IRQ_CMOS,
	IRQ_LEGACY1,
	IRQ_LEGACY2,
	IRQ_LEGACY3,
	IRQ_PS2_MOUSE,
	IRQ_FPU,
	IRQ_ATA1,
	IRQ_ATA2,
};

#endif
