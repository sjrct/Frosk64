//
// process.c
//
// written by sjrct
//

#include "process.h"
#include "interrupt.h"
#include "irq.h"
#include "paging.h"
#include "pagemgr.h"
#include "inline_asm.h"
#include "lock.h"

extern kern_obj * current_thread;

int procthrd_lock = 0;
kern_obj * head_process;
kern_obj * head_thread;

void init_processes(void)
{
	void context_switch();
	register_int(IRQ0 + IRQ_TIMER, (ulong)context_switch, KERNEL_CS, 0x8E);
}

kern_obj * spawn_process(const uchar * cd, ulong csz, ulong dsz, ulong bsz, kern_obj * par, uchar priv, uchar priority)
{
	ulong i, pbase, ssz, absz, pgs;
	kern_obj * proc = alloc_kobj();
	
	ssz = csz + dsz;
	absz = bsz;

	if (csz % 0x1000 != 0) csz = csz - (csz % 0x1000) + 0x1000;
	if (dsz % 0x1000 != 0) dsz = dsz - (dsz % 0x1000) + 0x1000;
	if (bsz % 0x1000 != 0) bsz = bsz - (bsz % 0x1000) + 0x1000;

	proc->type = KOBJ_PROCESS;
	proc->u.proc.parent = par;
	proc->u.proc.priv = priv;
	proc->u.proc.priority = priority;
	
	proc->u.proc.code_data_ws = alloc_ws();
	
	pgs = csz / 0x1000 + dsz / 0x1000 + bsz / 0x1000;
	pbase = alloc_pages(pgs + 0x100, PHYS_PAGES);
	if (pbase == 0) {
		free_kobj(proc);
		return NULL;
	}

	add_pgs_to_ws(proc->u.proc.code_data_ws, USPACE_BOT, pbase, csz + dsz + bsz + 0x100000, 7);

	for (i = 0; i < ssz + absz; i++) {
		if (i % 0x1000 == 0) {
			ATQ(KPT0_0_LOC) = (pbase + i) | 3;
			INVLPG(KSPACE_LOC);
		}
		if (i < ssz) ATB(KSPACE_LOC + i % 0x1000) = cd[i];
		else ATB(KSPACE_LOC + i % 0x1000) = 0;
	}

	if (spawn_thread(proc, (int (*)())USPACE_BOT) == NULL) {
		free_kobj(proc);
		free_pages(pbase, pgs, PHYS_PAGES);
		return NULL;
	}

	getlock(&procthrd_lock, 0);
	if (head_process == NULL) {
		proc->u.proc.next = proc;
		head_process = proc;
	} else {
		proc->u.proc.next = head_process->u.proc.next;
		head_process->u.proc.next = proc;
	}
	unlock(&procthrd_lock, 0);
	
	return proc;
}

kern_obj * spawn_thread(kern_obj * proc, int (*func)())
{
	ulong fl, pg;
	kern_obj * thrd = alloc_kobj();

	thrd->type = KOBJ_THREAD;
	thrd->u.thrd.proc = proc;
	
	thrd->u.thrd.sub = alloc_kobj();
	thrd->u.thrd.sub->type = KOBJ_THREAD_SUB;
	thrd->u.thrd.sub->u.thrd2.stack_ws = alloc_ws();

	pg = alloc_pages(2, PHYS_PAGES);
	if (pg == 0) {
		free_kobj(thrd->u.thrd.sub);
		free_kobj(thrd);
		return 0;
	}

	add_pgs_to_ws(thrd->u.thrd.sub->u.thrd2.stack_ws, USPACE_TOP - 0x2000, pg, 0x2000, 7);
	thrd->u.thrd.sub->u.thrd2.rsp = USPACE_TOP - 0x50;
	
	ATQ(KPT0_0_LOC) = (pg + 0x1000) | 3;
	INVLPG(KSPACE_LOC);	

	ATQ(KSPACE_LOC + 0xFB0) = (ulong)func;
	ATQ(KSPACE_LOC + 0xFB8) = USER_CS | 3;
	GET_FLAGS(fl);
	ATQ(KSPACE_LOC + 0xFC0) = fl;
	ATQ(KSPACE_LOC + 0xFC8) = USPACE_TOP - 0x1000;
	ATQ(KSPACE_LOC + 0xFD0) = USER_DS | 3;

	getlock(&procthrd_lock, 1);
	if (head_thread == NULL) {
		thrd->u.thrd.next = thrd;
		head_thread = thrd;
	} else {
		thrd->u.thrd.next = head_thread->u.thrd.next;
		head_thread->u.thrd.next = thrd;
	}
	unlock(&procthrd_lock, 1);

	return thrd;
}

void end_process(kern_obj * proc)
{
	// TODO
}

void end_thread(kern_obj * thrd)
{
	// TODO
}

kern_obj * get_current_proc(void)
{
	if (current_thread == NULL) return NULL;
	return current_thread->u.thrd.proc;
}
