//
// kernobj.h
//
// written by sjrct
//

#ifndef KERNOBJ_H
#define KERNOBJ_H

#include "kerndef.h"

enum {
	KOBJ_HEADER = 0,
	KOBJ_PHYSMEM,
	KOBJ_WORKSPACE,
	KOBJ_PROCESS,
	KOBJ_THREAD,
	KOBJ_THREAD_SUB,
	KOBJ_FSH_META,
};

#pragma pack(push, 1)
typedef struct kern_obj {
	uchar type;

	union {
		// all structures here must be 31 bytes

		struct {
			char pack[7];
			ulong bitmap[2];
			struct kern_obj * next;
		} hdr;

		struct {
			char type;
			char pack[6];
			ulong addr;
			ulong size;
			struct kern_obj * next;	
		} pmem;

		struct {
			char pack[1];
			ushort flags;
			uint size;
			ulong vbase;
			ulong pbase;
			struct kern_obj * next;
		} ws;

		struct {
			uchar priv;
			uchar priority;
			char pack[5];
			struct kern_obj * code_data_ws;
			struct kern_obj * parent;
			struct kern_obj * next;
		} proc;
		
		struct {
			uchar state;
			char pack[6];
			struct kern_obj * sub;
			struct kern_obj * proc;
			struct kern_obj * next;
		} thrd;
		
		struct {
			char pack[15];
			struct kern_obj * stack_ws;
			ulong rsp;
		} thrd2;
		
		struct {
			char pack[15];
			ulong addr;
			struct kern_obj * next;
		} fshm;
	} u;
} kern_obj;
#pragma pack(pop)

extern int kernobj_no_new_pages;

void init_kobj_mgr();
kern_obj * alloc_kobj();
void free_kobj(kern_obj *);
int verify_kobj(kern_obj *);

#endif
