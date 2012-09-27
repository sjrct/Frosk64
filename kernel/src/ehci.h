//
// ehci.h
//
// written by sjrct
//
// virtually all of the ehci driver has yet to be implemented
//

#ifndef EHCI_H
#define EHCI_H

#include <cmn/io.h>

typedef struct {
	uchar bus, dev, fun;
} ehci_pci_inf;

#define EHCI_FUNC_COUNT 2
extern int (*ehci_functions[EHCI_FUNC_COUNT])();

#endif
