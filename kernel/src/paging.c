//
// paging.c
//
// written by sjrct
//

#include "paging.h"
#include "interrupt.h"
#include "pagemgr.h"
#include "kerndef.h"
#include "inline_asm.h"

#define PRESENT_BIT   1
#define RW_BIT        2
#define PRIVLEDGE_BIT 4
#define ACCESSED_BIT  0x20
#define DIRTY_BIT     0x80

kern_obj * cur_ws[2] = {
	NULL, NULL
};

void init_paging(void)
{
	void page_fault();
	register_int(14, (ulong)page_fault, KERNEL_CS, 0x8E);
}

static ulong get_entry_addr(ulong baddr, ulong parent, uint shf)
{
	return parent + (((baddr >> shf) << 3) & 0xfff);
}

static void page_to_kpt0_0(ulong val)
{
	ATQ(KPT0_0_LOC) = val;
	INVLPG(KSPACE_LOC);
}

static ulong check_table(ulong baddr, ulong parent, uint shf, uint fl)
{
	uint i;
	ulong eaddr = get_entry_addr(baddr, parent, shf);
	ulong r = ATQ(eaddr);

	if (!(r & PRESENT_BIT)) {
		r = alloc_pages(1, PHYS_PAGES) | fl;

		ATQ(eaddr) = r;
		page_to_kpt0_0(r);
		
		for (i = 0; i < 0x1000; i++) ATB(KSPACE_LOC + i) = 0;
		
		r = KSPACE_LOC;
	} else {
		page_to_kpt0_0(r);
		r = KSPACE_LOC;
	}
		
	return r & ~0xfff;
}

static void set_pt_entry(ulong addr, ulong val, ulong fl)
{
	ulong t;

	if (addr >= KSPACE_LOC) {
		addr -= NON_CANON_SIZE;
	}
	
	t = check_table(addr, PML4T_LOC, 39, fl);
	t = check_table(addr, t, 30, fl);
	t = check_table(addr, t, 21, fl);

	t = get_entry_addr(addr, t, 12);

	ATQ(t) = val;
}

void page_in(ulong paddr, ulong vaddr, ulong size, uint fl)
{
	paddr = (paddr & ~0xfff) | fl;
	vaddr &= ~0xfff;
	if (size % 0x1000) size = (size & ~0xfff) + 0x1000;

	while (size > 0) {
		set_pt_entry(vaddr, paddr, fl);
		vaddr += 0x1000;
		paddr += 0x1000;
		size -= 0x1000;
	}
}

///////////////////////////////////////////////////////////////////////////////

kern_obj * alloc_ws(void)
{
	kern_obj * o = alloc_kobj();
	o->type = KOBJ_WORKSPACE;
	o->u.ws.size = 0;
	o->u.ws.next = NULL;
	return o;
}

void free_ws(kern_obj * ws)
{
	if (ws->u.ws.next != NULL) free_ws(ws->u.ws.next);
	free_kobj(ws);
}

void add_pgs_to_ws(kern_obj * o, ulong vbase, ulong pbase, uint size, ushort fl)
{
	while (o->u.ws.next != NULL) o = o->u.ws.next;

	if (o->u.ws.size != 0) {
		o->u.ws.next = alloc_ws();
		o = o->u.ws.next;
	}
	
	o->u.ws.flags = fl;
	o->u.ws.vbase = vbase;
	o->u.ws.pbase = pbase;
	o->u.ws.size = size;
}

void swap_ws(kern_obj * ws, int wsi)
{
	kern_obj * old;
	uint i;
	
	old = cur_ws[wsi];
	cur_ws[wsi] = ws;
	
	while (old != NULL) {
		for (i = 0; i < old->u.ws.size; i += 0x1000) {
			set_pt_entry(old->u.ws.vbase + i, 0, old->u.ws.flags);
			INVLPG(old->u.ws.vbase + i);
		}
		old = old->u.ws.next;
	}

	while (ws != NULL) {
		page_in(ws->u.ws.pbase, ws->u.ws.vbase, ws->u.ws.size, ws->u.ws.flags);		
		ws = ws->u.ws.next;
	}
}
