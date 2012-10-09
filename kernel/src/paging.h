//
// paging.h
//
// written by sjrct
//

#ifndef PAGING_H
#define PAGING_H

#include "cdef.h"
#include "kernobj.h"

void init_paging(void);
void page_in(ulong, ulong, ulong, uint);

kern_obj * alloc_ws(void);
void free_ws(kern_obj *);

//
// add_pgs_to_ws: Adds consecutive (physically and virtually) pages to a workspace
//
// Parameters:
//   kern_obj * workspace
//   ulong      virtual_base
//   ulong      physical_base
//   uint       size_in_bytes
//   ushort     pt_entry_flags
//
// The present bit (0x1) in pt_entry_flags should be set
//
void add_pgs_to_ws(kern_obj *, ulong, ulong, uint, ushort);

void swap_ws(kern_obj *, int);

#endif
