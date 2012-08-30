//
// physmem.h
//
// written by sjrct
//

#ifndef PHYSMEM_H
#define PHYSMEM_H

#include "kernobj.h"

void init_phys_mem();
kern_obj * alloc_phys_pgs(uint);
void free_phys_pgs(kern_obj *);

#endif
