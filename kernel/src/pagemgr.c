//
// pagemgr.c
//
// written by sjrct
//

#include "pagemgr.h"
#include "kerndef.h"

static kern_obj * head[MAX_PAGE_TYPES];

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

void init_page_mgr(void)
{
	uint i, sz;
	kern_obj * cur = NULL;
	kern_obj * prev;
	mem_map_node * mm;

	sz = *(uint*)(MEM_MAP_LOC);
	sz = (sz - MEM_MAP_LOC - 8) / 24;
	mm = (mem_map_node*)(MEM_MAP_LOC + 8);
	head[PHYS_PAGES] = NULL;

	for (i = 0; i < sz; i++, mm++) {
		if (mm->base != 0) {
			prev = cur;
			cur = alloc_kobj();
			cur->type = KOBJ_PAGES;
			cur->u.pmem.size = mm->size;
			cur->u.pmem.addr = mm->base;
			cur->u.pmem.type = mm->type;
			cur->u.pmem.next = NULL;

			if (head[PHYS_PAGES] != NULL) {
				prev->u.pmem.next = cur;
			} else {
				head[PHYS_PAGES] = cur;
			}
		}
	}
	
	head[KVIRT_PAGES] = alloc_kobj();
	head[KVIRT_PAGES]->type = KOBJ_PAGES;
	head[KVIRT_PAGES]->u.pmem.type = USABLE;
	head[KVIRT_PAGES]->u.pmem.size = 0x80000000;	// FIXME
	head[KVIRT_PAGES]->u.pmem.addr = KSPACE_LOC + 0x100000;
	head[KVIRT_PAGES]->u.pmem.next = NULL;
}

ulong alloc_pages(uint sz, page_type ty)
{
	ulong addr;
	kern_obj * cur = head[ty];
	kern_obj * prev = NULL;

	sz *= 0x1000;

	while (cur != NULL) {
		if (cur->u.pmem.size >= sz && cur->u.pmem.type == USABLE) {
			if (cur->u.pmem.size == sz) {
				if (prev == NULL) head[ty] = cur->u.pmem.next;
				else prev->u.pmem.next = cur->u.pmem.next;
				addr = cur->u.pmem.addr;
				free_kobj(cur);
				return addr;
			} else {
				cur->u.pmem.size = cur->u.pmem.size - sz;
				return cur->u.pmem.addr + cur->u.pmem.size;
			}
		}

		prev = cur;
		cur = cur->u.pmem.next;
	}

	return 0;
}

void free_pages(ulong addr, uint size, page_type ty)
{
	kern_obj * cur = head[ty];

	while (cur != NULL) {
		if (cur->u.pmem.addr + cur->u.pmem.size == addr) {
			cur->u.pmem.size += size;
			return;
		}

		if (addr + size == cur->u.pmem.addr) {
			cur->u.pmem.size += size;
			cur->u.pmem.addr = addr;
			return;
		}

		cur = cur->u.pmem.next;
	}

	cur = alloc_kobj();
	cur->type = KOBJ_PAGES;
	cur->u.pmem.type = USABLE;
	cur->u.pmem.size = size;
	cur->u.pmem.addr = addr;
	cur->u.pmem.next = head[ty];
	head[ty] = cur;
}
