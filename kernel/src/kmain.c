//
// kmain.c
//
// written by sjrct
//

#include "interrupt.h"
#include "exceptions.h"
#include "kernobj.h"
#include "pagemgr.h"
#include "paging.h"
#include "irq.h"
#include "inline_asm.h"
#include "fbe.h"
#include "fs.h"
#include "driver.h"
#include "syscall.h"
#include "tss.h"
#include "keyboard.h"

#include "cga_text.h"
#include "vesa.h"
#include "mouse.h"

#define START_PRGM_MAX_SIZE 0x1000

static void init_bss(void);

// defined in linker script
extern char * const _bss_addr;
extern const uint _bss_size;

void __attribute__ ((noreturn)) kmain()
{
	uchar buffer[START_PRGM_MAX_SIZE];
	handle_t hndl;

	init_bss();
	init_kobj_mgr();
	init_page_mgr();

	init_drivers();
	init_fs();
	
	reg_exceptions();
	init_irqs();
	init_paging();
	init_processes();
	init_syscalls();
	init_idt();
	init_tss();

	init_mouse();
	kb_init();
	init_graphics();

	hndl = fs_aquire("/sys/prgm/start/start", 0, 1);
	fs_read(hndl, (char*)buffer, START_PRGM_MAX_SIZE / 0x1000, 0);
	fs_release(hndl);

	exec_fbe(buffer, START_PRGM_MAX_SIZE, NULL, 0, 0xFF);
	start_timer();

	while(1) HLT;
}

static void init_bss(void)
{
	ulong i;
	for (i = 0; i < (ulong)&_bss_size; i++) {
		((char*)&_bss_addr)[i] = 0;
	}
}
