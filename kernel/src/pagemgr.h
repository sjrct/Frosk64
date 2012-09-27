//
// pagemgr.h
//
// written by sjrct
//

#ifndef PAGEMGR_H
#define PAGEMGR_H

#include "kernobj.h"

typedef enum {
	PHYS_PAGES = 0,
	KVIRT_PAGES,
	MAX_PAGE_TYPES
} page_type;

void init_page_mgr(void);
ulong alloc_pages(uint, page_type);
void free_pages(ulong, uint, page_type);

#endif
