//
// kmain.c
//
// written by sjrct
//

#include "interrupt.h"
#include "exceptions.h"
#include "kernobj.h"
#include "physmem.h"
#include "paging.h"
#include "sout.h"
#include "irq.h"
#include "inline_asm.h"
#include "process.h"
#include "keyboard.h"
#include "fs.h"
#include "ata.h"

#define START_PRGM_MAX_SIZE 0x1000

extern char * const _bss_addr;
extern const uint _bss_size;

void init_bss();

void __attribute__ ((noreturn)) kmain()
{
	fs_node * n;
	ushort buffer[START_PRGM_MAX_SIZE / 2];
	
	init_bss();
	init_kobj_mgr();
	init_phys_mem();
	init_fs();
	
	reg_exceptions();
	init_irqs();
	init_paging();
//	init_processes();
	init_keyboard();
	init_idt();
	
//	start_timer();

	n = fs_locate_node("/sys/prgm/start/start", NULL);
	n = fs_resolve_ptr(n->u.file.first_data_ptr);
	ata_read(n->u.data.lba * 8, n->u.data.blocks * 8, fs_drive, buffer);
	
	outn(ATQ(buffer), 0x10);

	while(1) HLT;
}

void init_bss()
{
	uint i;
	for (i = 0; i < _bss_size; i++) {
		_bss_addr[i] = 0;
	}
}
