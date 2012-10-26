//
// ports.h
//
// written by sjrct
//

#ifndef PORTS_H
#define PORTS_H

#include <cmn/utypes.h>
#include "kernobj.h"

void send(kern_obj *, int, const char *, ulong);
kern_obj * poll(int);
int recieve(kern_obj *, int, char *, ulong);

#endif
