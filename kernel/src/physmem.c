//
// physmem.c
//
// written by sjrct
//

#include "physmem.h"

static kern_obj * head;

#pragma pack(push, 1)
typedef struct {
	ulong base;
	ulong size;
	uint type;
	uint eflags;
} mem_map_node;
#pragma pack(pop)

enum memory_type {
	USABLE = 1,
	RESERVED,
	ACPI_RECLAIMABLE,
	ACPI_NVS,
	BAD
};

void init_phys_mem()
{
	uint i, sz;
	kern_obj * cur = NULL;
	kern_obj * prev;
	mem_map_node * mm;

	sz = *(uint*)(MEM_MAP_LOC);
	sz = (sz - MEM_MAP_LOC - 8) / 24;
	mm = (mem_map_node*)(MEM_MAP_LOC + 8);
	head = NULL;

	for (i = 0; i < sz; i++, mm++) {
		if (mm->base != 0) {
			prev = cur;
			cur = alloc_kobj();
			cur->type = KOBJ_PHYSMEM;
			cur->u.pmem.size = mm->size;
			cur->u.pmem.addr = mm->base;
			cur->u.pmem.type = mm->type;

			if (head != NULL) {
				prev->u.pmem.next = cur;
			} else {
				head = cur;
			}
		}
	}
}

kern_obj * alloc_phys_pgs(uint sz)
{
	kern_obj * new;
	kern_obj * cur = head;
	kern_obj * prev = NULL;;

	sz *= 0x1000;

	while (cur != NULL) {
		if (cur->u.pmem.size >= sz && cur->u.pmem.type == USABLE) {
			if (cur->u.pmem.size == sz) {
				if (prev == NULL) head = cur->u.pmem.next;
				else prev->u.pmem.next = cur->u.pmem.next;
				return cur;
			} else {
				new = alloc_kobj();
				new->type = KOBJ_PHYSMEM;
				new->u.pmem.size = sz;
				new->u.pmem.type = cur->u.pmem.type;
				cur->u.pmem.size = cur->u.pmem.size - sz;
				new->u.pmem.addr = cur->u.pmem.addr + cur->u.pmem.size;
				return new;
			}
		}

		prev = cur;
		cur = cur->u.pmem.next;
	}

	return NULL;
}

void free_phys_pgs(kern_obj * dat)
{
	kern_obj * cur = head;

	while (cur != NULL) {
		if (cur->u.pmem.addr + cur->u.pmem.size == dat->u.pmem.addr) {
			cur->u.pmem.size += dat->u.pmem.size;
			free_kobj(dat);
			return;
		}

		if (dat->u.pmem.addr + dat->u.pmem.size == cur->u.pmem.addr) {
			cur->u.pmem.size += dat->u.pmem.size;
			cur->u.pmem.addr = dat->u.pmem.addr;
			free_kobj(dat);
			return;
		}

		cur = cur->u.pmem.next;
	}

	dat->u.pmem.next = head;
	head = dat;
}
