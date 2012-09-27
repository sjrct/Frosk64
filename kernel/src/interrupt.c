//
// interrupt.c
//
// written by sjrct
//

#include "kerndef.h"
#include "interrupt.h"

#pragma pack(push, 1)
typedef struct idt_entry {
	ushort lowaddr;
	ushort selector;
	uchar  zero;
	uchar  flags;
	ushort midaddr;
	uint   highaddr;
	uint   zero2;
} idt_entry;

static idt_entry idt[0x100];

const struct {
	ushort size;
	ulong entries;
} idt_ptr = {
	sizeof idt,
	(ulong)idt
};
#pragma pack(pop)

void init_idt(void)
{
	asm volatile ("cli");
	asm volatile ("lidt idt_ptr(%rip)");
	asm volatile ("sti");
}

void register_int(int i, ulong addr, ushort sel, uchar flags)
{
	idt[i].lowaddr  = addr & 0xffff;
	idt[i].midaddr  = (addr >> 16) & 0xffff;
	idt[i].highaddr = addr >> 32;
	idt[i].selector = sel;
	idt[i].flags    = flags;
	idt[i].zero = idt[i].zero2 = 0;
}
