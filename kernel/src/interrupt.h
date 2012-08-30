//
// interrupt.h
//
// written by sjrct
//

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "cdef.h"

void init_idt();
void register_int(int, ulong, ushort, uchar);

#endif
