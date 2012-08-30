//
// process.c
//
// written by sjrct
//

#include "process.h"
#include "interrupt.h"
#include "irq.h"
#include "paging.h"
#include "physmem.h"
#include "inline_asm.h"

kern_obj * head_process;
kern_obj * head_thread;

void init_processes()
{
	void context_switch();
	register_int(IRQ0 + IRQ_TIMER, (ulong)context_switch, KERNEL_CS, 0x8E);
	enable_irq(IRQ_TIMER);
}

kern_obj * spawn_process(uchar * cd, ulong csz, ulong dsz, ulong bsz, kern_obj * par, uchar priv, uchar priority)
{
	ulong i, pbase, ssz, absz;
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
	
	if (head_process == NULL) proc->u.proc.next = proc;
	else proc->u.proc.next = head_process;
	head_process = proc;
	
	proc->u.proc.code_data_ws = alloc_ws();
	
	pbase = alloc_phys_pgs(csz / 0x1000 + dsz / 0x1000 + bsz / 0x1000)->u.pmem.addr;
	add_pgs_to_ws(proc->u.proc.code_data_ws, USPACE_BOT, pbase, csz + dsz + bsz, 3);
	
	for (i = 0; i < ssz + absz; i++) {
		if (i % 0x1000 == 0) {
			ATQ(KPT0_0_LOC) = (pbase + i) | 3;
			INVLPG(KSPACE_LOC);
		}
		if (i < ssz) ATB(KSPACE_LOC + i % 0x1000) = cd[i];
		else ATB(KSPACE_LOC + i % 0x1000) = 0;
	}

	spawn_thread(proc, (int (*)())USPACE_BOT);
	
	return proc;
}

kern_obj * spawn_thread(kern_obj * proc, int (*func)())
{
	ulong fl;
	kern_obj * thrd = alloc_kobj();

	thrd->type = KOBJ_THREAD;
	thrd->u.thrd.proc = proc;
	if (head_thread == NULL)  thrd->u.thrd.next = thrd;
	else thrd->u.thrd.next = head_thread;
	head_thread = thrd;
	
	thrd->u.thrd.sub = alloc_kobj();
	thrd->u.thrd.sub->type = KOBJ_THREAD_SUB;
	thrd->u.thrd.sub->u.thrd2.stack_ws = alloc_ws();
	add_pgs_to_ws(thrd->u.thrd.sub->u.thrd2.stack_ws, USPACE_TOP - 0x1000, alloc_phys_pgs(1)->u.pmem.addr, 0x1000, 3);
	thrd->u.thrd.sub->u.thrd2.rsp = USPACE_TOP - 0x50;
	
	ATQ(KPT0_0_LOC) = (USPACE_TOP - 0x1000) | 3;
	INVLPG(KSPACE_LOC);	

	ATQ(KSPACE_LOC + 0xFB0) = (ulong)func;
	ATQ(KSPACE_LOC + 0xFB8) = KERNEL_CS;
	GET_FLAGS(fl);
	ATQ(KSPACE_LOC + 0xFC0) = fl;
	ATQ(KSPACE_LOC + 0xFC8) = USPACE_TOP - 0x28;
	ATQ(KSPACE_LOC + 0xFD0) = KERNEL_DS;

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
