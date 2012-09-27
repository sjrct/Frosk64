//
// driver.h
//
// written by sjrct
//

#ifndef DRIVER_H
#define DRIVER_H

#include <cmn/fdrvr.h>
#include "cdef.h"

#define MAX_DRIVERS 0x100

//
// init_drivers: Calls init functions for all initial drivers
//
void init_drivers(void);

//
// driver_calld: Calls a driver directly
//
// Parameters:
//   uint driver_id
//   uint function_id
//   void * parameters
//
// Returns: Depends on driver and function
//
int driver_call(uint, uint, void *);

#endif
